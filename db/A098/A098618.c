/*
 * A098618  Products of A007482 and Catalan numbers: a(n) =
 *          A007482(n)*A000108(n).
 * https://oeis.org/A098618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007482(n) * Catalan(n) * A007482: a(0)=1, a(1)=3;
 *            a(n)=3*a(n-1)+2*a(n-2) * Catalan(n): C_0=1; C_{k+1}=C_k *
 *            (4*k+2)/(k+2) * The function returns a(0)=1, a(1)=3, a(2)=22, …
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

/* Computes the nth term of A007482 (offset 0).  */
static seq_t a007482_term(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)3;   /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = curr * (seq_t)3 + prev * (seq_t)2;
        prev = curr;
        curr = next;
    }
    return curr;
}

seq_t a(idx_t n)
{
    seq_t cat   = catalan(n);      /* k-th Catalan number */
    seq_t seq482 = a007482_term(n);/* kth term of A007482 */
    return seq482 * cat;
}
