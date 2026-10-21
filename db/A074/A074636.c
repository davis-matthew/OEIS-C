/*
 * A074636  Define b(k) by the recursion b(1)=n, b(k+1)=b(k)-floor(k/b(k)).
 *          Sequence gives the value a(n) such that b(a(n))=0; if k>a(n) then
 *          b(k) is undefined.
 * https://oeis.org/A074636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 166, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: b(1) = n,   b(k+1) = b(k) - ⌊k / b(k)⌋  (with the floor taken as in real numbers);  
   a(n) is the first k such that b(k) = 0.  For example a(1)=2, a(2)=5, a(3)=5, a(4)=100, … */
seq_t a(idx_t n)
{
    /* signed working value for the current b(k) */
    __int128 sb = n;          /* sb may become negative during the process */
    idx_t k = 1;              /* current k : we have b(k) already stored in sb */

    while (1) {
        /* a zero terminates the sequence */
        if (sb == 0)
            return (seq_t)k;   /* the position where b(k) == 0 is the value a(n) */

        /* compute ⌊ k / b(k) ⌋ correctly for possible negative b(k) */
        __int128 q = ( (__int128)k ) / sb;   /* truncation toward zero */
        __int128 r = ( (__int128)k ) % sb;   /* remainder with the sign of the numerator */

        /* adjust for floor when signs differ and remainder is non‑zero */
        if (r != 0 && (( (__int128)k ^ sb ) < 0))
            q -= 1;

        sb -= q;          /* b(k+1) = b(k) - ⌊k / b(k)⌋ */
        ++k;              /* next k */
    }
}
