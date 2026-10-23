/*
 * A023184  Least Fibonacci number ending with n.
 * https://oeis.org/A023184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   fib, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A023184 – Least Fibonacci number ending with n
 *
 * The computation is based on the fact that
 *   F_k ≡ n (mod 10^d)
 * where d = number of decimal digits of n.
 * The index k is found by iterating Fibonacci numbers modulo 10^d up
 * to the Pisano period for 10^d.  The period equals
 *    60          for d = 1
 *    300         for d = 2
 *    15·10^(d-1) for d ≥ 3
 * Once k is found, the exact Fibonacci number F_k is returned.
 *
 * This method works for all supported build types: 8-, 16-, 32-, 64-bit
 * unsigned integers, 128-bit unsigned integers, and arbitrary‑precision
 * integers.  The arithmetic uses only integer operators permitted by the
 * requirements.
 */

#include "../utils/fib.h"
#include "../utils/pow10.h"

/* Returns the least Fibonacci number whose decimal representation ends
 * with the digits of n (including leading zeros if any).  The index
 * n is 0‑based as in the OEIS entry. */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Count decimal digits of n */
    idx_t temp = n;
    idx_t d = 0;
    while (temp > (idx_t)0) {
        ++d;
        temp /= (idx_t)10;
    }

    /* Modulus m = 10^d */
    seq_t mod = pow10(d);

    /* Target value (mod m) */
    seq_t target = (seq_t)n;

    /* Fibonacci residues modulo m */
    seq_t f0 = (seq_t)0;   /* F_0 */
    seq_t f1 = (seq_t)1;   /* F_1 */

    /* Compute the Pisano period for 10^d */
    idx_t period;
    if (d == (idx_t)1) {
        period = (idx_t)60;
    } else if (d == (idx_t)2) {
        period = (idx_t)300;
    } else {
        /* period = 15 * 10^(d-1) */
        seq_t p10 = pow10(d - (idx_t)1);
        period = (idx_t)(p10 * (seq_t)15);
    }

    /* Search for the first k with F_k ≡ n (mod 10^d) */
    for (idx_t k = (idx_t)0; k <= period; ++k) {
        if (f0 == target)
            return fib(k);          /* F_k found */
        if (f1 == target)
            return fib(k + (idx_t)1);/* F_{k+1} found */

        /* Advance to next Fibonacci residue modulo mod */
        seq_t f_next = f0 + f1;
        if (f_next >= mod)
            f_next %= mod;

        f0 = f1;
        f1 = f_next;
    }

    /* Should never happen for valid n; return 0 as a safe fallback. */
    return (seq_t)0;
}
