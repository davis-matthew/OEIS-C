/*
 * A138606  List first F(1) odd numbers, then first F(2) even numbers
 *          (starting from 2), then the next F(3) odd numbers, then the next
 *          F(4) even numbers, etc., where F(n) = A000045(n), the n-th
 *          Fibonacci number.
 * https://oeis.org/A138606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) enumerates odds and evens in Fibonacci-sized groups
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t total_terms = 0;      /* number of terms already processed */
    idx_t used_odds   = 0;      /* odds used so far */
    idx_t used_evens  = 0;      /* evens used so far */

    idx_t f_prev = 0;           /* Fibonacci numbers: f_prev = F(k-1), f_cur = F(k) */
    idx_t f_cur  = 1;
    idx_t group = 1;            /* current group index (1-based) */

    while (1) {
        idx_t group_size = f_cur;          /* size of the current group */

        /* Does the requested term fall in this group? */
        if (total_terms + group_size >= n) {
            idx_t offset1 = n - total_terms; /* 1‑based position inside the group */

            if (group & 1) { /* odd group */
                return (seq_t)2 * (seq_t)(used_odds + offset1) - (seq_t)1;
            } else {        /* even group */
                return (seq_t)2 * (seq_t)(used_evens + offset1);
            }
        }

        /* Otherwise, consume the entire group and move to the next one */
        total_terms += group_size;
        if (group & 1) used_odds += group_size;
        else          used_evens += group_size;

        /* Advance Fibonacci numbers for the next group */
        idx_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = f_next;
        ++group;
    }
}
