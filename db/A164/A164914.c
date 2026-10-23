/*
 * A164914  a(n) = A164853(n+1)/A164853(n).
 * https://oeis.org/A164914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p if the (n+1)-th semiprime is 2·p or p²; otherwise
 *            a(n)=1
 * Range:     u8 n <= 21, u16 n <= 5983, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/floor_sqrt.h"

/* static storage for precomputed data */
static uint64_t *semiprimes = NULL;      /* list of semiprimes (0‑based)    */
static size_t   semiprime_count = 0;     /* number of stored semiprimes     */
static idx_t   *spf_array   = NULL;      /* smallest prime factor array     */
static idx_t     sieve_limit = 0;        /* limit used for sieve            */

/* simple prime test using the pre‑computed spf array */
static inline bool is_prime_val(idx_t v)
{
    return v >= 2 && spf_array[v] == v;
}

/* build the sieve and semiprime list up to a safe limit */
static void init_sequence(void)
{
    const idx_t LIMIT = 200000;          /* large enough for n up to ~10 000   */
    sieve_limit = LIMIT;

    /* allocate and compute spf array (smallest prime factor)  */
    spf_array = (idx_t *)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= LIMIT; ++i) spf_array[i] = 0;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (spf_array[i] == 0) {        /* i is prime */
            for (idx_t j = i; j <= LIMIT; j += i) {
                if (spf_array[j] == 0) spf_array[j] = i;
            }
        }
    }
    /* set spf for primes > sqrt(LIMIT) themselves */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf_array[i] == 0) spf_array[i] = i;
    }

    /* generate the list of semiprimes up to LIMIT  */
    size_t cap   = 50000;               /* initial capacity */
    uint64_t *list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t   cnt    = 0;

    for (idx_t num = 4; num <= LIMIT; ++num) {
        idx_t temp = num;
        int   factors = 0;              /* count of prime factors w. multiplicity */

        while (temp > 1) {
            idx_t p = spf_array[temp];
            while (temp % p == 0) {
                ++factors;
                temp /= p;
                if (factors > 2) break;
            }
            if (factors > 2) break;
        }
        if (factors == 2) {
            if (cnt >= cap) {
                cap <<= 1;
                list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
            }
            list[cnt++] = (uint64_t)num;
        }
    }
    list = (uint64_t *)realloc(list, cnt * sizeof(uint64_t));

    semiprimes     = list;
    semiprime_count = cnt;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    /* per OEIS offset: n >= 1  */
    assert(n < semiprime_count);            /* safety check */

    uint64_t s = semiprimes[n];              /* (n+1)-th semiprime (0‑based) */

    if ((s & 1ULL) == 0) {                  /* even → 2·p  */
        uint64_t p = s >> 1;                /* s / 2      */
        if (p <= sieve_limit && is_prime_val((idx_t)p))
            return (seq_t)p;
    } else {                                /* odd → possible p²  */
        seq_t root = floor_sqrt((seq_t)s);   /* integer sqrt  */
        if ((uint64_t)root * (uint64_t)root == s
            && is_prime_val((idx_t)root))
            return (seq_t)root;
    }

    return (seq_t)1;
}
