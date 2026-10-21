/*
 * A057337  1) Write the Zeckendorf expression of n; 2) Remove initial 1 and
 *          append a final 0; 3) Replace numbers in this as follows: 0 -> 1,
 *          010 -> 2, 01010 -> 3, 0101010 -> 4...; 4) Find a binary number
 *          with run lengths from step 3 (starting with 1); 5) The term a(n)
 *          is the decimal equivalent of this binar...
 * https://oeis.org/A057337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A057337:  a(n) is computed from the Zeckendorf representation
 * of n by the steps described in the OEIS entry.
 *
 *   1) Write Zeckendorf representation of n (binary string of Fibonacci
 *      indices, starting with a leading 1).
 *   2) Remove that leading 1 and append a final 0.
 *   3) Split the resulting string into tokens of the form 0(10)^k
 *      (k ≥ 0).  Replace each token by k+1.
 *   4) Interpret the sequence of the obtained numbers as run lengths
 *      of an alternating binary string starting with a run of 1s.
 *   5) Convert that binary string to decimal – that is a(n).
 *
 *   Offset = 1.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ----- 1.  Build Fibonacci numbers (Zeckendorf base) ----- */
    enum { MAX_FIB = 100 };             /* > 93 – plenty for 64‑bit n   */
    idx_t fib[MAX_FIB];
    size_t fib_len = 0;

    /* F2 = 1, F3 = 2, then Fk = Fk-1 + Fk-2 */
    fib[fib_len++] = (idx_t)1;          /* F2 */
    if (n >= (idx_t)2) {
        fib[fib_len++] = (idx_t)2;      /* F3 */
        while (fib_len < MAX_FIB) {
            idx_t nxt = fib[fib_len - 1] + fib[fib_len - 2];
            if (nxt > n) break;
            fib[fib_len++] = nxt;
        }
    }

    /* ----- 2.  Zeckendorf representation (bits) ----- */
    char bits[MAX_FIB];
    for (size_t i = 0; i < fib_len; ++i)
        bits[i] = 0;

    idx_t rest = n;
    for (int i = (int)fib_len - 1; i >= 0; --i) {
        if (fib[i] <= rest) {
            bits[i] = 1;
            rest -= fib[i];
        }
    }

    /* ----- 3.  Remove the leading 1, append final 0 ----- */
    /* The new string has exactly fib_len characters. */
    char s[200];
    size_t slen = 0;
    for (int i = (int)fib_len - 2; i >= 0; --i)
        s[slen++] = bits[i] ? '1' : '0';
    s[slen++] = '0';

    /* ----- 4‑5.  Tokenise, build run lengths, and compute the value ----- */
    seq_t result = 0ULL;          /* start with zero */
    bool ones_run = true;         /* first run is a run of 1s */
    size_t pos = 0;

    while (pos < slen) {
        /* s[pos] must be '0' of the next token */
        int pairs = 0;
        ++pos;                    /* skip the leading zero of the token */

        /* count how many "10" pairs follow */
        while (pos + 1 < slen && s[pos] == '1' && s[pos + 1] == '0') {
            ++pairs;
            pos += 2;
        }

        int runlen = pairs + 1;   /* token value */

        result <<= runlen;        /* shift left to make room for the run */
        if (ones_run) {
            /* append a run of 1s – mask of runlen ones */
            result |= (((seq_t)1 << runlen) - (seq_t)1);
        }
        ones_run = !ones_run;     /* alternate runs */
    }

    return result;
}
