/*
 * A226722  Positions of the numbers 2^n, for n >=0, in the joint ranking of
 *          all the numbers 2^h, 3^k, 5^k, for h >= 0, k >= 1.
 * https://oeis.org/A226722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-1)*log_3(2)) + floor((n-1)*log_5(2))
 * Range:     u8 n <= 124, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fixed‑point approximations of log_3(2) and log_5(2) with 1 000 000 precision (≈5·10⁻⁷ error)    */
/*  log_3(2)  ≈ 0.6309297535714574  → 630929 / 1 000 000            */
/*  log_5(2)  ≈ 0.4306765580733933  → 430677 / 1 000 000            */
static const uint64_t LOG3_NUM = 630929;   /* numerator for log_3(2) */
static const uint64_t LOG3_DEN = 1000000;  /* denominator for log_3(2) */
static const uint64_t LOG5_NUM = 430677;   /* numerator for log_5(2) */
static const uint64_t LOG5_DEN = 1000000;  /* denominator for log_5(2) */

/* floor((n‑1)*log_3(2)) computed with fixed‑point arithmetic             */
static idx_t floor_log3(idx_t n_minus1)
{
    unsigned __int128 prod = (unsigned __int128)n_minus1 * LOG3_NUM;
    return (idx_t)(prod / LOG3_DEN);
}

/* floor((n‑1)*log_5(2)) computed with fixed‑point arithmetic             */
static idx_t floor_log5(idx_t n_minus1)
{
    unsigned __int128 prod = (unsigned __int128)n_minus1 * LOG5_NUM;
    return (idx_t)(prod / LOG5_DEN);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safe guard – undefined for n<1      */
    idx_t m  = n - 1;
    idx_t k3 = floor_log3(m);
    idx_t k5 = floor_log5(m);

    seq_t result = (seq_t)n;
    result += (seq_t)k3;
    result += (seq_t)k5;
    return result;
}
