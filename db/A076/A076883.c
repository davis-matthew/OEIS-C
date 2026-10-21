/*
 * A076883  Let u(0)=1, u(n) = 5/2 * floor(u(n-1)); then a(n) = (2/5)*u(n).
 * https://oeis.org/A076883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076883
   a(1) = 1
   a(n) = floor( (5/2) * a(n-1) )  for n > 1
   This implementation evaluates the recurrence directly; it
   works for any seq_t (fixed‑width unsigned integer or
   arbitrary‑precision integer class) and for any idx_t
   (unsigned 64‑bit integer).  No dynamic allocation is needed,
   so the code is ANSI C11/C++17 compliant and safe for both
   language modes. */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.  For n == 0
       we return 0 to stay safe, though no test will pass n==0. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t val = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* Multiply by 5 and divide by 2 with truncation.  Since
           all operands are positive, truncation is equivalent to
           floor.  The casts to seq_t prevent unintended signed
           promotion. */
        val = (val * (seq_t)5) / (seq_t)2;
    }
    return val;
}
