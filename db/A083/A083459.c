/*
 * A083459  Palindromes arising in A083458.
 * https://oeis.org/A083459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 + A083458(n)^5
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A083458(n) = 1, 2, 6, then 10^(n-3) for n ≥ 4 */
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* For n < 1 the sequence is undefined – return 0 as a safe fallback */
    if (n == 0) return (seq_t)0;

    /* Handle the first three special cases */
    if (n == 1) return (seq_t)2;          /* 1 + 1^5 = 2 */
    if (n == 2) return (seq_t)33;         /* 1 + 2^5 = 33 */
    if (n == 3) return (seq_t)7777;       /* 1 + 6^5 = 7777 */

    /* For n ≥ 4: A083458(n) = 10^(n-3) */
    /* So a(n) = 1 + (10^(n-3))^5 = 1 + 10^(5*(n-3)) */
    idx_t exp = 5 * (n - 3);
    seq_t p = pow10(exp);
    return p + (seq_t)1;
}
