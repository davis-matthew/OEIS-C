/*
 * A078217  Smallest multiple of n that begins with the concatenation of the
 *          positive integers <= n and coprime to n (in increasing order).
 * https://oeis.org/A078217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A078217:  a(n) = the smallest multiple of n that starts with
 *  the concatenation of all integers ≤ n that are coprime to n.
 *
 *  Algorithm:
 *      1. Build the concatenation C of all k (1≤k≤n) with gcd(k,n)=1.
 *      2. Let r_d = (-C * 10^d) mod n.
 *         Find the least d with r_d < 10^d.
 *      3. The answer is C * 10^d + r_d.
 *
 *  No external utilities are required; everything is computed with
 *  standard integer arithmetic and the generic seq_t type.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* Helper: number of decimal digits of x (x≥1) */
static int digits_of(idx_t x)
{
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* Helper: multiply a by 10 repeatedly d times (seq_t) */
static seq_t pow10_seq(int d)
{
    seq_t p = 1;
    for (int i = 0; i < d; ++i) p *= 10;
    return p;
}

/* Helper: Euclidean GCD on 64‑bit values, used only for n≤uint64_t */

seq_t a(idx_t n)
{
    /* 1. Construct the concatenation C of all integers ≤ n coprime to n. */
    seq_t C = 0;
    uint64_t n64 = (uint64_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd_uint64((uint64_t)k, n64) == 1) {
            int d = digits_of(k);
            seq_t mult = pow10_seq(d);
            C = C * mult + (seq_t)k;
        }
    }

    /* 2. Compute C mod n, cast to 64‑bit for modular arithmetic. */
    seq_t Cmod_seq = C % (seq_t)n;
    uint64_t Cmod = (uint64_t)Cmod_seq;   /* Cmod < n ≤ 2^64-1 */

    /* 3. Search for the smallest d with remainder < 10^d. */
    uint64_t pow10_mod = 1 % n64;   /* 10^d mod n */
    seq_t pow10_seq_val = 1;        /* 10^d as seq_t */

    for (int d = 0; ; ++d) {
        /* r = (-Cmod * 10^d) mod n  = (n - (Cmod*10^d)%n) % n */
        uint64_t prod_mod = ( (unsigned __int128)Cmod * pow10_mod ) % n64;
        uint64_t need = (n64 - prod_mod) % n64;      /* 0 ≤ need < n */

        if ((seq_t)need < pow10_seq_val) {
            /* answer = C * 10^d + need */
            return C * pow10_seq_val + (seq_t)need;
        }

        /* next d: multiply 10 modulo n, and as seq_t */
        pow10_mod = (pow10_mod * 10) % n64;
        pow10_seq_val *= 10;
    }
}
