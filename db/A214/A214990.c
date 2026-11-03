/*
 * A214990  Second nearest integer to n*r, where r = (1+ sqrt(5))/2, the
 *          golden ratio.
 * https://oeis.org/A214990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*φ) if (n + floor(n*√5)) is odd, floor(n*φ)+1 if
 *            (n + floor(n*√5)) is even, where φ = (1+√5)/2 and √5 is the
 *            square root of 5
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9705 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the second nearest integer to n*φ.
   All computations are performed with integer arithmetic only.  */

#include <stdint.h>
#include <stddef.h>

/* Auxiliary routine: compare mid*mid with A represented as three 64‑bit words
   hi:mid:lo  (little endian).  Return 1 if mid*mid > A, 0 otherwise. */
static int mid_sq_greater_than_A(uint64_t mid,
                                 uint64_t A_hi, uint64_t A_mid, uint64_t A_lo)
{
    /* Decompose mid into high (≤3) and low (64‑bit) parts */
    uint64_t mh = mid >> 64;              /* 0…3 */
    uint64_t ml = (uint64_t)mid;          /* low 64 bits */

    /* high part of the product */
    uint64_t high = mh * mh;              /* ≤9 */

    /* cross term: 2 * mh * ml   (≤ 6 * (2^64–1) < 2^67) */
    unsigned __int128 cross = (unsigned __int128)mh * ml * 2ULL;
    uint64_t cross_lo = (uint64_t)cross;
    uint64_t cross_hi = (uint64_t)(cross >> 64);   /* 0…3 */

    /* low part: ml * ml   (128 bits) */
    unsigned __int128 lo_prod = (unsigned __int128)ml * ml;
    uint64_t prod_lo = (uint64_t)lo_prod;
    uint64_t prod_mid = (uint64_t)(lo_prod >> 64);

    /* Combine the three parts: hi:mid:lo   (≤ 132 bits) */
    uint64_t mid_res = prod_mid + cross_lo;
    uint64_t carry = 0;
    if (mid_res < prod_mid) carry = 1;      /* overflow from addition */

    uint64_t hi_res = high + cross_hi + carry;

    /* Compare the 3‑word values */
    if (hi_res > A_hi) return 1;
    if (hi_res < A_hi) return 0;
    if (mid_res > A_mid) return 1;
    if (mid_res < A_mid) return 0;
    if (prod_lo > A_lo) return 1;
    return 0;
}

/* Computes floor(sqrt(5*n^2)).  The result fits in 66 bits, returned in
   a 64‑bit unsigned integer.  All intermediate stages use 128‑bit values
   only; the final result is never larger than 2^66. */
static uint64_t floor_sqrt5_mul_n(uint64_t n)
{
    /* Compute 5*n*n in 128‑bit, then split into three 64‑bit words:
       hi:mid:lo (lo = low 64, mid = next 64, hi = high bits, ≤4).   */
    unsigned __int128 prod = (unsigned __int128)n * n;   /* ≤ 2^128‑1 */
    uint64_t lo = (uint64_t)prod;
    uint64_t hi128 = (uint64_t)(prod >> 64);            /* high 64 bits */

    /* Multiply by 5 */
    unsigned __int128 low5 = (unsigned __int128)lo * 5ULL;
    uint64_t low5_lo = (uint64_t)low5;
    uint64_t low5_hi = (uint64_t)(low5 >> 64);          /* ≤5   */

    uint64_t high5_low = hi128 * 5ULL;                  /* may need 67 bits */
    uint64_t high5_hi = high5_low >> 64;                /* ≤3   */
    high5_low &= 0xFFFFFFFFFFFFFFFFULL;                /* keep low 64 bits */

    /* Assemble A = 5*n*n as 3 words */
    uint64_t A_lo = low5_lo;
    uint64_t A_mid = low5_hi + high5_low;
    uint64_t carry_mid = 0;
    if (A_mid < low5_hi) carry_mid = 1;                 /* overflow mid part */
    uint64_t A_hi = high5_hi + carry_mid;

    /* Binary search for floor(sqrt(A)) on [0, 3*n] (exclusive upper bound). */
    uint64_t low = 0;
    uint64_t high;
    if (n > UINT64_MAX/3) high = UINT64_MAX;           /* overflow guard   */
    else high = 3*n + 1;                               /* exclusive bound */
    while (low + 1 < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid_sq_greater_than_A(mid, A_hi, A_mid, A_lo))
            high = mid;
        else
            low = mid;
    }
    return low;  /* floor(sqrt(5*n^2)) */
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Compute t = floor(n*sqrt(5)) */
    uint64_t t = floor_sqrt5_mul_n(n);

    /* Compute sum = n + t  (fits in 65 bits) */
    unsigned __int128 sum = (unsigned __int128)n + (unsigned __int128)t;

    /* floor(n*φ) = floor(sum/2) */
    unsigned __int128 floor_phi = sum >> 1;

    /* a(n) = floor_phi + (sum even ? 1 : 0)   */
    seq_t result = (seq_t)floor_phi;
    if ((uint64_t)sum & 1ULL) { /* sum is odd → use floor_phi (floor case) */
        /* no addition */
    } else {
        result = (seq_t)(floor_phi + 1);  /* even → ceil case */
    }
    return result;
}
