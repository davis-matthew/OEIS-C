/*
 * A084901  a(n) = 4^(n-2)*n*(5*n+3)/2.
 * https://oeis.org/A084901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fact: a(n) = 4^(n-2)*n*(5*n+3)/2 */
/* Equivalent recurrence (valid for n ≥ 3):
   a(n) = 12*a(n-1) - 48*a(n-2) + 64*a(n-3)
   with initial terms a(0)=0, a(1)=1, a(2)=13 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle the offset terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;

    /* Compute using the 3‑term linear recurrence */
    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)13;  /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = 12 * a2 - 48 * a1 + 64 * a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
