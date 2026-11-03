/*
 * A196082  Greatest residue of x^n (mod n), x=0..n-1.
 * https://oeis.org/A196082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Greatest residue of x^n (mod n), x = 0..n-1 (OEIS
 *            A196082)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

/*
 * Computes the a(n) value for OEIS sequence A196082.
 * The function uses the provided mod_pow routine for modular exponentiation.
 * For prime n it returns n-1 immediately; for powers of two it returns 1.
 * For any other n it scans all residues x^n (mod n) to find the maximum.
 */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                          /* a(1) = 0 */

    /* power of two: a(2^p) = 1 */
    if ((n & (n - 1)) == 0)
        return 1;

    /* prime: a(n) = n - 1 */
    if (is_prime(n))
        return (seq_t)n - 1;

    seq_t mod = (seq_t)n;
    seq_t max_res = 0;
    seq_t n_minus_1 = mod - 1;

    for (idx_t x = 0; x < n; ++x) {
        seq_t r = powmod((seq_t)x, n, mod);
        if (r > max_res)
            max_res = r;
        if (max_res == n_minus_1)            /* cannot be larger */
            break;
    }
    return max_res;
}
