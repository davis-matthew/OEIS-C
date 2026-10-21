/*
 * A072995  Least k > 0 such that the number of solutions to x^k == 1 (mod k)
 *          1 <= x <= k is equal to n, or 0 if no such k exists.
 * https://oeis.org/A072995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1013, u64 n <= 1013,
 *            u128 n <= 1013, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least k > 0 such that the number of solutions of
 * x^k ≡ 1 (mod k) for 1 ≤ x ≤ k equals n,
 * or 0 if no such k exists.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* gcd for 64‑bit unsigned integers */
static inline uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* up to this value we pre‑compute all results */
#define MAX_K 2000000ULL

static bool initialized = false;
static uint32_t *spf = NULL;     /* smallest prime factor */
static uint32_t *mink_for_count = NULL; /* min k for each count */

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate arrays */
    spf = (uint32_t *)malloc((MAX_K + 1) * sizeof(uint32_t));
    mink_for_count = (uint32_t *)calloc((MAX_K + 1), sizeof(uint32_t));
    if (!spf || !mink_for_count) return;  /* memory error */

    /* linear sieve for smallest prime factors */
    spf[0] = spf[1] = 1;
    for (uint32_t i = 2; i <= MAX_K; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_K) {
                for (uint32_t j = i * i; j <= MAX_K; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* count solutions for k = 1 separately */
    mink_for_count[1] = 1;   /* f(1) = 1 */

    /* compute f(k) for k >= 2 */
    for (uint32_t k = 2; k <= MAX_K; ++k) {
        uint32_t tmp = k;
        uint32_t count = 1;
        uint32_t last_p = 0, exp = 0;

        while (tmp > 1) {
            uint32_t p = spf[tmp];
            if (p != last_p) {
                if (last_p != 0) {
                    uint64_t term;
                    if (last_p == 2) {
                        if (exp == 1) term = 1;
                        else if (exp == 2) term = ugcd(k, (uint64_t)2);
                        else term = 2 * ugcd(k, (uint64_t)1ULL << (exp - 2));
                    } else {
                        uint64_t p_pow = 1;
                        for (uint32_t i = 0; i < exp; ++i) p_pow *= last_p;
                        uint64_t phi = p_pow - p_pow / last_p;
                        term = ugcd(k, phi);
                    }
                    if (term == 0) term = 1;    /* safety */
                    count *= (uint32_t)term;
                }
                last_p = p;
                exp = 1;
            } else {
                ++exp;
            }
            tmp /= p;
        }

        /* handle last prime factor */
        uint64_t term;
        if (last_p == 2) {
            if (exp == 1) term = 1;
            else if (exp == 2) term = ugcd(k, (uint64_t)2);
            else term = 2 * ugcd(k, (uint64_t)1ULL << (exp - 2));
        } else {
            uint64_t p_pow = 1;
            for (uint32_t i = 0; i < exp; ++i) p_pow *= last_p;
            uint64_t phi = p_pow - p_pow / last_p;
            term = ugcd(k, phi);
        }
        if (term == 0) term = 1;
        count *= (uint32_t)term;

        if (count <= MAX_K && mink_for_count[count] == 0)
            mink_for_count[count] = k;
    }
}

seq_t a(idx_t n)
{
    init();
    if (n > MAX_K) return (seq_t)0;
    return (seq_t)mink_for_count[(uint32_t)n];
}
