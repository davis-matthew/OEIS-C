/*
 * A052897  Expansion of e.g.f.: exp(2*x/(1-x)).
 * https://oeis.org/A052897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the e.g.f. exp(2*x/(1-x)):
   a(0) = 1, a(1) = 2,
   a(n+2) = (4 + 2*n) * a(n+1) - (n^2 + n) * a(n)   for n ≥ 0
   (n^2 + n) is computed as (seq_t)n * ((seq_t)n + 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev = (seq_t)1;   /* a(n-2) */
    seq_t a_curr = (seq_t)2;   /* a(n-1) */

    for (idx_t i = 0; i + 2 <= n; ++i) {
        seq_t coef1 = (seq_t)4 + (seq_t)2 * (seq_t)i;            /* 4 + 2*i */
        seq_t coef2 = (seq_t)i * ((seq_t)i + (seq_t)1);          /* i*(i+1) */
        seq_t a_next = coef1 * a_curr - coef2 * a_prev;          /* a(n+2) */

        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
