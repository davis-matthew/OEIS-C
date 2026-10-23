/*
 * A022810  a(n) = L(n+2) + c(n) where L(k) is the k-th Lucas number and c(n)
 *          is the n-th number that is 1 or 3 or is not a Lucas number.
 * https://oeis.org/A022810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n+2) + c(n) L(k) is the k‑th Lucas number (L(0)=2,
 *            L(1)=1, L(n)=L(n‑1)+L(n‑2)). c(n) is the n‑th positive integer
 *            that is 1 or 3 or not a Lucas number (i.e. all numbers except
 *            Lucas numbers greater than 3). Index offset is 1: a(1) = 5
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* ---- compute L(n+2) ------------------------------------------- */
    seq_t l_prev2 = (seq_t)2;   /* L(0) */
    seq_t l_prev1 = (seq_t)1;   /* L(1) */
    for (idx_t i = 2; i <= n + 2; ++i) {
        seq_t next = l_prev1 + l_prev2;  /* L(i) = L(i-1)+L(i-2) */
        l_prev2 = l_prev1;
        l_prev1 = next;
    }
    seq_t lucas = l_prev1;   /* L(n+2) */

    /* ---- compute c(n) --------------------------------------------- */
    /* We generate Lucas numbers on the fly. curLucas holds the
       next Lucas number (>3) to be compared. */
    seq_t lprev2_luc = (seq_t)1;  /* L(1) */
    seq_t lprev1_luc = (seq_t)3;  /* L(2) */
    seq_t curLucas   = lprev2_luc + lprev1_luc;  /* L(3) = 4 */

    seq_t target   = (seq_t)n;   /* we need the n‑th such number */
    seq_t counted  = (seq_t)0;   /* how many numbers have been counted so far */
    seq_t x        = (seq_t)1;   /* current candidate integer */

    while (1) {
        /* Skip Lucas numbers larger than 3 */
        if (x == curLucas) {
            /* Advance Lucas state: curLucas becomes the next one after current */
            seq_t newPrev2 = lprev1_luc;
            seq_t newPrev1 = curLucas;
            lprev2_luc = newPrev2;
            lprev1_luc = newPrev1;
            curLucas     = lprev1_luc + lprev2_luc;  /* next Lucas number */

            ++x;   /* move to next integer after the skipped one */
            continue;
        }

        /* Count this integer towards c(n) */
        ++counted;
        if (counted == target) {
            /* We found the n-th number in the sequence.  Return sum. */
            return lucas + x;
        }

        ++x;
        /* No need to update curLucas here; it will be handled
           when x becomes equal to curLucas in the next loop. */
    }
}
