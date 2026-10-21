/*
 * A055930  Number of distinct prime factors of totient of (n-th prime)!.
 * https://oeis.org/A055930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of φ(pₙ!), where pₙ is
 *            the n‑th prime. The computation uses the fact that φ(pₙ!) =
 *            ∏_{q≤pₙ} q^{e_q-1} (q-1) and therefore its prime divisors are
 *            exactly: • all primes q ≤ pₙ/2 (since e_q ≥ 2), and • all prime
 *            divisors of (q‑1) for each prime q ≤ pₙ
 * Range:     u8 n <= 457, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, factorize, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factorize.h"

/* Helper to set a bit in a byte array */
static inline void set_bit(uint8_t *arr, size_t idx) { arr[idx] = 1; }

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* n ≥ 1 for the OEIS offset */

    /* n‑th prime pₙ */
    uint32_t pn = get_prime(n);

    /* primes up to pₙ */
    uint64_t *prime_list = NULL;
    size_t   prime_cnt  = 0;
    build_primes_upto((idx_t)pn, &prime_list, &prime_cnt);
    if (!prime_list) return 0;             /* allocation failure fallback */

    /* allocation for a boolean array marking prime divisors */
    size_t setsize = (size_t)pn + 1;
    uint8_t *have = (uint8_t*)malloc(setsize);
    if (!have) {
        free(prime_list);
        return 0;                          /* allocation failure fallback */
    }
    memset(have, 0, setsize);

    /* 1. Mark all primes q ≤ pₙ/2  (they contribute q^{e_q-1}) */
    uint32_t half = pn / 2;                 /* integer division is fine */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t q = (uint32_t)prime_list[i];
        if (q <= half) set_bit(have, q);
    }

    /* 2. For each prime q ≤ pₙ, factor q‑1 and mark its prime divisors */
    uint64_t facpr[64];
    int      exps[64];
    size_t   fcnt;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        uint64_t val = q - 1;
        if (val <= 1) continue;            /* skip 1 */
        fcnt = factorize(val, facpr, exps);
        for (size_t j = 0; j < fcnt; ++j) {
            uint64_t f = facpr[j];
            /* f is ≤ q‑1 ≤ pₙ‑1, so it fits in our array index */
            set_bit(have, (size_t)f);
        }
    }

    /* 3. Count distinct primes marked */
    seq_t result = 0;
    for (size_t i = 2; i <= pn; ++i) {
        if (have[i]) ++result;
    }

    /* Clean up */
    free(have);
    free(prime_list);
    return result;
}
