/*
 * A073408  Let cophi_m(x) denotes the cototient function applied m times to x
 *          (cophi(x)=x-phi(x)). Sequence gives the minimum number of
 *          iterations m such that cophi_m(n) divides n.
 * https://oeis.org/A073408
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimal m ≥ 1 such that cophi_m(n) divides n (cophi_m(n)
 *            = repeated application of x → x – φ(x) )
 * Range:     u8 n <= 2317, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 121 terms through n = 9627 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 2 in OEIS offset 2; any value below is undefined,
       but returning 0 keeps the function safe for all inputs. */
    if (n < 2) return 0;

    seq_t N   = (seq_t)n;   /* original number */
    seq_t cur = N;          /* current value of iterated cophi */

    for (idx_t m = 1; ; ++m) {
        seq_t phi_cur = totient_seq(cur);
        seq_t next    = cur - phi_cur;   /* cophi(cur) */

        /* Next should never be zero for n ≥ 2, but guard against it. */
        if (next == 0) return (seq_t)m;

        if (N % next == 0) return (seq_t)m;

        cur = next;   /* prepare for next iteration */
    }
}
