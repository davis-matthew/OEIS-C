/*
 * A209686  For each n, define a sequence of numbers by S(0)=n, S(i) = sum of
 *          last three digits of the concatenation S(0)S(1)S(2)...S(i-1); a(n)
 *          = smallest m such that S(m) = 11, or -1 if 11 is never reached.
 * https://oeis.org/A209686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  for a given starting value n
   build the sequence S(i) with S(0)=n and
   S(i) = sum of the decimal digits that are the last
   three digits of the concatenation S(0)S(1)...S(i-1).
   Return the smallest index i with S(i)=11,
   or -1 if 11 is never reached.  The state of the
   process is fully described by the current 3‑digit
   “look‑back” buffer; detecting a repeated state
   guarantees that 11 will never appear. */

#include <stdbool.h>

static int digits_of_seq_t(seq_t value, int out[3])
{
    /* fills out[0..len-1] with most‑significant first digits;
       returns the number of digits (1..3) */
    if (value == 0) {
        out[0] = 0;
        return 1;
    }
    int rev[3];
    int rlen = 0;
    while (value > 0 && rlen < 3) {
        rev[rlen++] = (int)(value % 10);
        value = value / 10;
    }
    for (int i = 0; i < rlen; ++i)
        out[i] = rev[rlen - 1 - i];
    return rlen;
}

static int encode_state(const int buf[3], int len)
{
    /* Unique code for a state, value in [0,3999] */
    int digits = 0;
    for (int i = 0; i < len; ++i)
        digits = digits * 10 + buf[i];
    return len * 1000 + digits;
}

seq_t a(idx_t n)
{
    /* 11 is immediately reached when the start value is 11 */
    if (n == 11)
        return 0;

    /* Initialise the left‑most–only buffer with the last
       up to three decimal digits of the initial n */
    int buf[3];
    int len;
    seq_t last_three = n % 1000;          /* works for any n >= 0 */
    len = digits_of_seq_t(last_three, buf);

    /* visited states – code range 0..3999 */
    static bool visited[4000];
    for (int i = 0; i < 4000; ++i)
        visited[i] = false;
    visited[encode_state(buf, len)] = true;

    int idx = 0;                          /* S(0) already produced */

    for (;;) {
        /* Sum the digits in the buffer to get the next term */
        int sum = 0;
        for (int i = 0; i < len; ++i)
            sum += buf[i];
        seq_t next = (seq_t)sum;

        ++idx;
        if (next == 11)
            return idx;

        /* Convert next to its decimal digits (most significant first) */
        int nd[3];
        int nlen = digits_of_seq_t(next, nd);

        /* Append those digits to the buffer, keeping only the last 3 */
        for (int i = 0; i < nlen; ++i) {
            if (len < 3)
                buf[len++] = nd[i];
            else {
                buf[0] = buf[1];
                buf[1] = buf[2];
                buf[2] = nd[i];
            }
        }

        int code = encode_state(buf, len);
        if (visited[code])
            return -1;                  /* loop detected – 11 unreachable */
        visited[code] = true;
    }
}
