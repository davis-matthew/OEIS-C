/*
 * A044928  a(n)=so-se, where so(se)=sum of odd(even) base 6 run lengths of n.
 * https://oeis.org/A044928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum of odd-length base‑6 run lengths minus sum of
 *            even‑length * run lengths of the base‑6 representation of n
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t odd_sum  = 0;
    seq_t even_sum = 0;

    /* Handle n == 0 (not part of the sequence, but harmless). */
    if (n == 0) return 0;

    idx_t x = n;
    int last_digit = -1;
    int run = 0;

    while (x > 0) {
        int d = (int)(x % 6);   /* current base‑6 digit */
        if (d == last_digit) {
            run++;              /* extend current run */
        } else {
            if (last_digit != -1) {          /* first run finished */
                if (run & 1)
                    odd_sum += (seq_t)run;
                else
                    even_sum += (seq_t)run;
            }
            last_digit = d;  /* start new run */
            run = 1;
        }
        x /= 6;              /* move to next digit */
    }

    /* Add the final run after loop ends. */
    if (last_digit != -1) {
        if (run & 1)
            odd_sum += (seq_t)run;
        else
            even_sum += (seq_t)run;
    }

    return odd_sum - even_sum;
}
