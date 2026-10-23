/*
 * A175177  Conjectured number of numbers for which the iteration x -> phi(x)
 *          + 1 terminates at prime(n). Cardinality of rooted tree T_p (where
 *          p is n-th prime) in Karpenko's book.
 * https://oeis.org/A175177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of rooted tree T_{p_n} where p_n is the n-th prime,
 *            defined by the iteration x -> phi(x)+1
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ---------- Prime sieve ---------- */
#define PRIME_LIMIT 4000000
static uint32_t primes[400000];
static size_t prime_count = 0;
static uint8_t is_composite[PRIME_LIMIT + 1];
static int primes_inited = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    prime_count = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_count++] = i;
            if (i <= PRIME_LIMIT / i) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    primes_inited = 1;
}

/* ---------- binary search for first prime >= value ---------- */
static size_t lower_bound_prime(uint64_t minp)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint64_t)primes[mid] < minp)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---------- hash table for memoisation of subtree sizes ---------- */
#define HASH_SIZE 262144   /* must be power of two */
static uint64_t memo_key[HASH_SIZE];
static seq_t memo_val[HASH_SIZE];
static int memo_inited = 0;

static size_t hash_pos(uint64_t key)
{
    size_t h = key & (HASH_SIZE - 1);
    while (memo_key[h] != 0 && memo_key[h] != key)
        h = (h + 1) & (HASH_SIZE - 1);
    return h;
}

/* ---------- preimage generation: numbers x with phi(x)=t ---------- */
#define MAX_PREIMAGE 1024
static void preimage_rec(uint64_t t, uint64_t minp,
                         uint64_t *out, size_t *cnt)
{
    if (t == 1) {
        /* phi(x)=1 => x=1 or 2, but only 2 uses prime 2 */
        size_t k = 0;
        out[k++] = 1;
        if (minp <= 2) out[k++] = 2;
        *cnt = k;
        return;
    }
    size_t total = 0;
    size_t idx = lower_bound_prime(minp);
    for (; idx < prime_count && (uint64_t)primes[idx] - 1 <= t; ++idx) {
        uint64_t p   = primes[idx];
        uint64_t diff = p - 1;
        if (t % diff != 0) continue;
        uint64_t t1 = t / diff;

        uint64_t pk_minus1 = 1;   /* p^(k-1) */
        uint64_t pk        = p;   /* p^k */
        for (;;) {
            if (t1 % pk_minus1 != 0) break;
            uint64_t t2 = t1 / pk_minus1;
            uint64_t sub[MAX_PREIMAGE];
            size_t subcnt;
            preimage_rec(t2, p + 1, sub, &subcnt);
            for (size_t s = 0; s < subcnt && total < MAX_PREIMAGE; ++s) {
                out[total++] = sub[s] * pk;
            }
            /* next exponent */
            if (pk_minus1 > t1 / p) break;   /* avoid overflow or too big */
            /* pk_minus1 = p^(k-1) becomes pk for next iteration */
            pk_minus1 = pk;
            if (pk > UINT64_MAX / p) break;  /* avoid overflow of pk */
            pk *= p;
        }
    }
    *cnt = total;
}

/* ---------- compute subtree size with memoisation ---------- */
static seq_t compute_size(uint64_t node)
{
    size_t h = hash_pos(node);
    if (memo_key[h] == node)
        return memo_val[h];

    seq_t sum = 1;   /* count the node itself */
    if (node > 1) {
        uint64_t t = node - 1;
        uint64_t buf[MAX_PREIMAGE];
        size_t cnt;
        preimage_rec(t, 2, buf, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t child = buf[i];
            if (child == node) continue;   /* avoid double counting root */
            sum += compute_size(child);
        }
    }

    memo_key[h] = node;
    memo_val[h] = sum;
    return sum;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* initialise data structures only once */
    if (!primes_inited) init_primes();
    if (!memo_inited) {
        for (size_t i = 0; i < HASH_SIZE; ++i)
            memo_key[i] = 0;
        memo_inited = 1;
    }

    /* get n-th prime (1‑based index) */
    uint64_t p = primes[(size_t)n - 1];
    return compute_size(p);
}
