/*
 * A184157  The sum of the even distances in the rooted tree with
 *          Matula-Goebel number n.
 * https://oeis.org/A184157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code computes A184157 – the sum of even distances in the rooted
    tree with Matula–Goebel number n – by evaluating recursively the tree
    for each Matula number.  For a node t the tree is obtained by
    factoring t into primes.  Each prime p = p_m corresponds to a child
    subtree of Matula number m.  The statistics kept for a subtree are

        sz   – number of nodes
        cE, cO – counts of nodes at even/odd distance from the root
        sE, sO – sums of distances to the nodes of even/odd parity
        sumEven – a(t), the required value for the subtree

    The recurrence processes all children in any order, maintaining an
    accumulated statistic of the part of the tree already processed.  For
    each child copy the contribution of cross‑subtree pairs whose total
    distance is even is added together with the contribution of the
    child subtree itself.  No explicit root‑child handling is needed,
    because it is covered by the cross‑pairs when the accumulator
    initially consists of only the root.  Memoisation guarantees
    O(number of distinct Matula numbers) time per call.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Sequenced types are supplied by the build system.  */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* --------------------------------------------------------------------- */
/*  Prime sieve and rank                                                        */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 2000000U   /* sufficient for n up to about 10^6      */

static bool primes_inited = false;
static size_t prime_cnt = 0;
static uint32_t *primes = NULL;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    /* Allocate and build sieve.  */
    bool *is_prime = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t p = 2; (uint64_t)p * p <= SIEVE_LIMIT; ++p)
        if (is_prime[p])
            for (uint64_t q = (uint64_t)p * p; q <= SIEVE_LIMIT; q += p)
                is_prime[(size_t)q] = false;

    /* Count primes.  */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[(size_t)i]) ++prime_cnt;

    /* Store primes in sorted array.  */
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) abort();
    size_t idx = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[(size_t)i])
            primes[idx++] = i;

    free(is_prime);
}

/* Return 1‑based index of prime p in the prime list (p must be prime).  */
static size_t prime_index(uint32_t p)
{
    init_primes();
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t midv = primes[mid];
        if (midv < p) lo = mid + 1;
        else if (midv > p) hi = mid;
        else return mid + 1;
    }
    return 0; /* not found – should not happen for valid input */
}

/* --------------------------------------------------------------------- */
/*  Statistics for a subtree.                                               */
/* --------------------------------------------------------------------- */
typedef struct {
    seq_t sz;      /* number of nodes in this subtree */
    seq_t cE, cO;  /* counts of nodes at even / odd distance from root */
    seq_t sE, sO;  /* sums of distances to nodes at even / odd distances */
    seq_t sumEven; /* a(n) for this subtree */
} Stats;

/* --------------------------------------------------------------------- */
/*  Memoisation table.                                                      */
/* --------------------------------------------------------------------- */
static Stats *stats_cache = NULL;
static size_t cache_cap = 0;

/* Ensure cache large enough to hold entry for n.  */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    Stats *new_cache = (Stats *)realloc(stats_cache,
                                        new_cap * sizeof(Stats));
    if (!new_cache) abort();
    /* New entries indicate uninitialised entries by sz == 0.  */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i].sz = 0;
    stats_cache = new_cache;
    cache_cap = new_cap;
}

/* Forward declaration.  */
static Stats *get_stats(idx_t n);

/* --------------------------------------------------------------------- */
/*  Recursive evaluation of a Stat block for Matula number n.              */
/* --------------------------------------------------------------------- */
static Stats *get_stats(idx_t n)
{
    if (n == 0) return NULL;           /* never called with 0 */
    ensure_cache_size(n);
    Stats *cur = &stats_cache[(size_t)n];
    if (cur->sz != 0) return cur;      /* already computed */

    /* Base case: single vertex.  */
    if (n == 1) {
        cur->sz      = 1;
        cur->cE      = 1;      /* root itself (distance 0) */
        cur->cO      = 0;
        cur->sE      = 0;
        cur->sO      = 0;
        cur->sumEven = 0;
        return cur;
    }

    /* Factorise n using the prime list.  */
    uint64_t rem = n;
    uint32_t factors[64];
    unsigned exps[64];
    size_t num_factors = 0;
    init_primes();

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= rem; ++i) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            factors[num_factors] = p;
            exps[num_factors]    = e;
            ++num_factors;
        }
    }
    if (rem > 1) {
        factors[num_factors] = (uint32_t)rem;
        exps[num_factors]    = 1;
        ++num_factors;
    }

    /* Accumulated statistics for the part already processed.  */
    seq_t acc_cE = 1;  /* root itself (distance 0) */
    seq_t acc_cO = 0;
    seq_t acc_sE = 0;
    seq_t acc_sO = 0;
    seq_t acc_sz = 1;
    seq_t sumEven_total = 0;

    for (size_t fi = 0; fi < num_factors; ++fi) {
        uint32_t prime_factor = factors[fi];
        unsigned times = exps[fi];
        size_t child_idx = prime_index(prime_factor);   /* Matula number of child */
        Stats *child = get_stats((idx_t)child_idx);
        for (unsigned k = 0; k < times; ++k) {
            /* Statistics of the child as seen from this root.  */
            seq_t child_cE = child->cO;                 /* even after shift */
            seq_t child_cO = child->cE;                 /* odd after shift */
            seq_t child_sE_flip = child->sO + child->cO; /* sum to new even nodes */
            seq_t child_sO_flip = child->sE + child->cE; /* sum to new odd nodes */

            /* Cross pairs between accumulated part and this child.  */
            seq_t cross_sum =
                acc_sE * child_cE +                        /* even acc with even child */
                acc_sO * child_cO +                        /* odd acc with odd child */
                acc_cE * child_sE_flip +                   /* even acc with new even nodes */
                acc_cO * child_sO_flip;                    /* odd acc with new odd nodes */

            sumEven_total += child->sumEven + cross_sum;

            /* Update accumulated statistics.  */
            acc_cE += child_cE;
            acc_cO += child_cO;
            acc_sE += child_sE_flip;
            acc_sO += child_sO_flip;
            acc_sz += child->sz;
        }
    }

    /* Store results for this tree.  */
    cur->sz      = acc_sz;
    cur->cE      = acc_cE;
    cur->cO      = acc_cO;
    cur->sE      = acc_sE;
    cur->sO      = acc_sO;
    cur->sumEven = sumEven_total;
    return cur;
}

/* --------------------------------------------------------------------- */
/*  Public interface.                                                      */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    Stats *s = get_stats(n);
    return s->sumEven;
}
