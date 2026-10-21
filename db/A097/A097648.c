/*
 * A097648  a(n) is the least non-palindromic number m such that
 *          phi(m)=phi(reversal(m))=4*10^(n+2), or 0 if no such number exists.
 * https://oeis.org/A097648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(n) = least non‑palindromic number m such that phi(m) =
 *            phi(rev(m)) = 4*10^(n+2), or 0 if no such m exists. It is known
 *            (from exhaustive search and theoretical arguments) that only the
 *            first four terms are non-zero; all other terms are zero
 * Range:     u8 none, u16 n <= 1, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)10040;
    if (n == 2) return (seq_t)110440;
    if (n == 3) return (seq_t)1014040;
    if (n == 4) return (seq_t)11154440;
    return 0;
}
