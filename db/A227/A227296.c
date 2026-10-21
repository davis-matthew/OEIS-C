/*
 * A227296  Number of partitions of n into parts <= phi(n), where phi is
 *          Euler's totient function (cf. A000010).
 * https://oeis.org/A227296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts ≤ φ(n) (φ = Euler’s
 *            totient)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* forward declarations of the utilities that may be used if desired */
#include "../utils/factor_u64.h"

/* Pre‑computed small primes up to 100000 (enough for n ≤ 10000). */
static const uint32_t small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
    109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
    293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
    367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431,
    433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491,
    499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
    577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641,
    643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709,
    719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787,
    797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859,
    863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
    947, 953, 967, 971, 977, 983, 991, 997
};
static const size_t small_primes_cnt = sizeof(small_primes)/sizeof(small_primes[0]);

/* Compute Euler’s totient of n (n ≤ 10 000) using the small prime list. */
static seq_t phi_small(idx_t n)
{
    if (n < 2) return (seq_t)n;
    seq_t result = n;
    idx_t temp = n;
    for (size_t i = 0; i < small_primes_cnt && (idx_t)small_primes[i] * (idx_t)small_primes[i] <= temp; ++i) {
        uint32_t p = small_primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)  /* remaining prime factor */
        result -= result / temp;
    return result;
}

/* Allocate an array of seq_t and ensure each element is zero‑initialised.  */
static seq_t *alloc_zero_array(idx_t size)
{
    seq_t *arr;
#ifdef __cplusplus
    arr = new seq_t[size]();   /* value‑initialise every element */
#else
    arr = (seq_t*)calloc((size_t)size, sizeof(seq_t));
#endif
    return arr;
}

static void free_array(seq_t *arr)
{
#ifdef __cplusplus
    delete [] arr;
#else
    free(arr);
#endif
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* Special case n = 0 – by convention partitions of 0 is 1. */
    if (n == 0) return (seq_t)1;

    seq_t phi_n = phi_small(n);          /* compute φ(n) */
    idx_t max_part = (idx_t)phi_n;
    if (max_part > n) max_part = n;      /* safety – φ(n) cannot exceed n */

    seq_t *dp = alloc_zero_array(n + 1);
    if (!dp) return (seq_t)0;            /* allocation failure – unlikely */

    dp[0] = (seq_t)1;                    /* base case: empty partition */

    /* Standard integer partition DP with restricted part size. */
    for (idx_t part = 1; part <= max_part; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n];
    free_array(dp);
    return result;
}
