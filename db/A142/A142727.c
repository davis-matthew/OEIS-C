/*
 * A142727  For definition see Comments lines.
 * https://oeis.org/A142727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1) = 1.  For n ≥ 2 let p = prime(n),
   a(n) is the (p‑1)st element of the nondecreasing multiset S
   which initially equals N = {1,2,3,…}.  After a(n) is chosen,
   one copy of every multiple of a(n) is added to S.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- parameters ----------------------------------------------------*/
#define MAX_N         10000          /* maximum n we expect to handle */
#define MAX_PRIME_LIMIT 110000      /* enough for the first 10000 primes */
#define MAX_PRIMES     20000
#define MAX_M          200000        /* size of the Fenwick tree */

/*--- globals -------------------------------------------------------*/
static uint32_t   primes[MAX_PRIMES];
static size_t     prime_cnt = 0;

static seq_t     a_val[MAX_N + 2];      /* store computed a(n), 1‑indexed */
static idx_t     a_max_computed = 0;    /* highest n for which a_val[n] known */

static seq_t     fenw[MAX_M + 2];      /* Fenwick tree, 1‑indexed */
static idx_t     fenw_size = 0;        /* current size of fenwick */

/*--- Fenwick tree helpers ----------------------------------------*/
static seq_t fenw_query(idx_t i) {
    seq_t sum = (seq_t)0;
    for (; i > 0; i -= i & -i) sum += fenw[i];
    return sum;
}

static void fenw_add(idx_t i, seq_t delta) {
    for (; i <= fenw_size; i += i & -i) fenw[i] += delta;
}

/*--- prime sieve --------------------------------------------------*/
static void init_primes(void) {
    static int done = 0;
    if (done) return;
    done = 1;

    bool *is_prime = (bool*)malloc((MAX_PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) return; /* unlikely, but guard */

    for (idx_t i = 0; i <= MAX_PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (idx_t i = 2; i <= MAX_PRIME_LIMIT && prime_cnt < MAX_PRIMES; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;

    free(is_prime);
}

/*--- Fenwick initialization --------------------------------------*/
static void init_fenwick(void) {
    static int done = 0;
    if (done) return;
    done = 1;
    fenw_size = MAX_M;

    /* Initially each natural number has multiplicity 1 */
    for (idx_t i = 1; i <= fenw_size; ++i)
        fenw_add(i, (seq_t)1);
}

/*--- sequence generator ------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, harmless */

    if (a_max_computed == 0) {            /* lazy initialization */
        init_primes();
        init_fenwick();
        a_val[1] = (seq_t)1;
        a_max_computed = 1;
    }

    if (n <= a_max_computed) return a_val[n];

    /* compute missing terms up to n */
    for (idx_t k = a_max_computed + 1; k <= n; ++k) {
        idx_t prime_idx = k - 1;                  /* 0‑based index into primes */
        seq_t target = (seq_t)primes[prime_idx] - (seq_t)1; /* (p−1) */
        /* binary search for the smallest x with prefix sum ≥ target */
        idx_t lo = 1, hi = fenw_size;
        while (lo < hi) {
            idx_t mid = lo + ((hi - lo) >> 1);
            if (fenw_query(mid) >= target) hi = mid;
            else                            lo = mid + 1;
        }
        seq_t val = (seq_t)lo;
        a_val[k] = val;

        /* add one copy of every multiple of val to the multiset */
        idx_t step = (idx_t)val;                 /* cast seq_t to idx_t */
        if (step != 0) {
            for (idx_t m = step; m <= fenw_size; m += step)
                fenw_add(m, (seq_t)1);
        }
    }

    a_max_computed = n;
    return a_val[n];
}
