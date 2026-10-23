/*
 * A129720  Number of 0's in odd position in all Fibonacci binary words of
 *          length n. A Fibonacci binary word is a binary word having no 00
 *          subword.
 * https://oeis.org/A129720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A129720 (OEIS A129720): Number of 0's in odd
   positions in all Fibonacci binary words of length n.
   Let f0[n] = number of such words ending with 0,
       f1[n] = number ending with 1,
       g0[n] = total 0's in odd positions of words ending with 0,
       g1[n] = total 0's in odd positions of words ending with 1.
   Recurrence:
     f0[n] = f1[n-1];
     f1[n] = f0[n-1] + f1[n-1];
     g0[n] = g1[n-1] + f1[n-1] * ([n is odd] ? 1 : 0);
     g1[n] = g0[n-1] + g1[n-1];
   Then a(n) = g0[n] + g1[n].
   Base (n=1): f0=1, f1=1, g0=1, g1=0. a(0)=0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t f0 = (seq_t)1, f1 = (seq_t)1;
    seq_t g0 = (seq_t)1, g1 = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t prev_f0 = f0, prev_f1 = f1;
        seq_t prev_g0 = g0, prev_g1 = g1;
        seq_t new_f0 = prev_f1;
        seq_t new_f1 = prev_f0 + prev_f1;
        seq_t add_even = (i & 1) ? (seq_t)1 : (seq_t)0; /* i odd */
        seq_t new_g0 = prev_g1 + prev_f1 * add_even;
        seq_t new_g1 = prev_g0 + prev_g1;
        f0 = new_f0; f1 = new_f1;
        g0 = new_g0; g1 = new_g1;
    }
    return g0 + g1;
}
