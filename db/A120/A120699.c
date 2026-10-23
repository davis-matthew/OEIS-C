/*
 * A120699  Lengths of set partitions.
 * https://oeis.org/A120699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120699: Lengths of set partitions.
   Each integer m appears Bell(m) times.
   a(n) = smallest m such that  ∑_{k=0}^{m} Bell(k) > n.
   Bell(m) computed via recurrence:
     B(0)=1,
     B(n+1)=∑_{k=0}^{n} C(n,k)*B(k).                                  */
#include "../utils/binom.h"

static seq_t bell[32];        /* cache of Bell numbers */
static idx_t bell_len = 0;    /* number of Bell numbers cached */

/* Ensure Bell numbers are known up to index n. */
static void compute_bell_upto(idx_t n)
{
    if (n < bell_len) return;              /* already ready */
    for (idx_t t = bell_len; t <= n; ++t) {
        if (t == 0) {
            bell[0] = 1;
        } else {
            seq_t s = 0;
            for (idx_t k = 0; k < t; ++k)
                s += binom(t - 1, k) * bell[k];
            bell[t] = s;
        }
    }
    bell_len = n + 1;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;            /* cumulative sum of Bell numbers */
    for (idx_t t = 0; ; ++t) {
        compute_bell_upto(t);
        sum += bell[t];
        if (sum > (seq_t)n)
            return (seq_t)t; /* value at index n */
    }
}
