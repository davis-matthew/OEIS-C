/*
 * A080884  Sum of (0,1) versions of Thue-Morse word (A010060) and Fibonacci
 *          word (A003849).
 * https://oeis.org/A080884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Thue‑Morse(n) + FibonacciWord(n)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth value of the Fibonacci word (1‑based index) */
static seq_t fibonacci_word_value(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* w0 = "0"  */
    if (n == 2) return (seq_t)1;   /* w1 = "01" */

    idx_t lens[64];
    lens[0] = 1;   /* length of w0 */
    lens[1] = 2;   /* length of w1 */
    idx_t cap = 2;
    while (lens[cap-1] < n) {
        lens[cap] = lens[cap-1] + lens[cap-2];
        cap++;
    }

    idx_t posi = n;
    idx_t pos  = cap - 1;
    while (pos > 1) {
        if (posi <= lens[pos-1]) {
            pos--;
        } else {
            posi -= lens[pos-1];
            pos -= 2;
        }
    }

    /* pos == 0 → w0 (single '0') */
    if (pos == 0) return (seq_t)0;

    /* pos == 1 → w1 = "01" */
    return (posi == 1) ? (seq_t)0 : (seq_t)1;
}

/* Return the nth value of the Thue–Morse sequence (0‑based index) */
static seq_t thue_morse_value(idx_t n)
{
    /* parity of popcount(n‑1) */
    return (seq_t)((__builtin_popcountll(n-1)) & 1);
}

seq_t a(idx_t n)
{
    seq_t t = thue_morse_value(n);
    seq_t f = fibonacci_word_value(n);
    return t + f;
}
