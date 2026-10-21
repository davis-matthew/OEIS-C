/*
 * A089080  Sequence is S(infinity) where S(1)={1,2} and S(n)=S(n-1)S'(n-1),
 *          where S'(k) is obtained from S(k) by replacing the single 1 with
 *          the least integer not occurring in S(k).
 * https://oeis.org/A089080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=2; for n>2 let t = ceil(log₂ n); let m = t+1; let
 *            k=n-2^{t-1}. Then a(n) = a(k) if a(k)!=1, else m. This
 *            implements the recursive construction of the sequence
 *            S(1)={1,2}, S(n)=S(n-1)S'(n-1) where S'(k) replaces the single 1
 *            in S(k) with the least integer not occurring in S(k)
 * Range:     u8 n <= 9944, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint32_t */

static seq_t a_rec(idx_t n, idx_t len);

/* Main accessor */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Determine smallest power of two >= n */
    idx_t len = 1;
    while (len < n) len <<= 1;

    return a_rec(n, len);
}

/* Recursive helper – len is a power of two >= n */
static seq_t a_rec(idx_t n, idx_t len)
{
    if (len == 2) {
        return (n == 1) ? (seq_t)1 : (seq_t)2;
    }

    idx_t half = len >> 1;
    if (n <= half) {
        /* First half – unchanged */
        return a_rec(n, half);
    } else {
        /* Second half – copy with possible replacement of 1 */
        idx_t k = n - half;
        seq_t val = a_rec(k, half);
        if (val == (seq_t)1) {
            /* Determine missing integer for this level:
               missing = log₂(len) + 1
            */
            uint32_t t = 0;
            idx_t tmp = len;
            while (tmp > 1) {
                tmp >>= 1;
                t++;
            }
            return (seq_t)(t + 1);
        } else {
            return val;
        }
    }
}
