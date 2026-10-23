/*
 * A145307  Numbers x such that there exists n in N : (x+79)^3-x^3=n^2.
 * https://oeis.org/A145307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145307: Numbers x such that (x+79)^3 - x^3 is a square.
   Recurrence (valid for n ≥ 1 with offset 1):
       a(1) = 7663
       a(2) = 3514673818
       a(n+2) = 456302*a(n+1) - a(n) + 18023850
   This function computes a(n) directly by iterating the recurrence,
   with no external caching or dynamic memory allocation. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    if (n == 1) return (seq_t)7663;
    if (n == 2) return (seq_t)3514673818ULL;

    seq_t prevprev = (seq_t)7663;          /* a(1) */
    seq_t prev     = (seq_t)3514673818ULL;  /* a(2) */
    seq_t curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)456302 * prev - prevprev + (seq_t)18023850;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
