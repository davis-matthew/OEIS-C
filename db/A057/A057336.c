/*
 * A057336  1) Write n in binary; 2) Find run lengths of this expression; 3)
 *          Replace these as follows: 1 -> 0, 2 -> 010, 3 -> 01010, 4 ->
 *          0101010...; 4) Remove final 0 and append an initial 1; 5) The term
 *          a(n) is the number with the obtained Zeckendorf expression.
 * https://oeis.org/A057336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by converting the binary representation of n
 *            into * a Zeckendorf representation via run‑length analysis: * 1.
 *            Write n in binary. * 2. Replace consecutive runs of equal bits
 *            of length k by the pattern * (10) repeated k−1 times followed by
 *            0 (e.g. k = 3 → 01010). * 3. Concatenate a...
 * Range:     u8 n <= 63, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the trivial case n == 0 (outside of the sequence's offset). */
    if (n == 0) return 0;

    /* Step 1: determine the binary bit length of n. */
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }
    if (len == 0) len = 1;           /* n == 0 would give len 0 */

    /* Step 2: obtain run lengths of consecutive bits. */
    int runs[64];                    /* maximum runs equal to len <= 64 */
    int runcnt = 0;
    int cur_bit = 1;                 /* leading bit of n is always 1 for n>=1 */
    int cur_run = 1;
    for (int pos = len - 2; pos >= 0; --pos) {
        int bit = (int)((n >> pos) & 1);
        if (bit == cur_bit) {
            ++cur_run;
        } else {
            runs[runcnt++] = cur_run;
            cur_bit = bit;
            cur_run = 1;
        }
    }
    runs[runcnt++] = cur_run;

    /* Step 3: build the concatenated pattern bits. */
    uint8_t orig_bits[256];
    int orig_len = 0;
    for (int i = 0; i < runcnt; ++i) {
        int k = runs[i];
        for (int j = 0; j < 2 * k - 1; ++j) {
            orig_bits[orig_len++] = (uint8_t)((j & 1) ? 1 : 0);
        }
    }

    /* Remove the final 0 (guaranteed to exist). */
    if (orig_len == 0) { /* never happens for n>=1 but guard */
        return 0;
    }
    int trimmed_len = orig_len - 1;

    /* Prepend a leading 1. */
    uint8_t final_bits[256];
    int final_len = 0;
    final_bits[final_len++] = 1;
    for (int i = 0; i < trimmed_len; ++i) {
        final_bits[final_len++] = orig_bits[i];
    }

    /* Step 5: interpret as Zeckendorf representation. */
    /* Precompute Fibonacci numbers F2=1, F3=2, ... up to final_len terms. */
    seq_t fib[256];
    fib[0] = (seq_t)1;                 /* F2 */
    if (final_len > 1) fib[1] = (seq_t)2; /* F3 */
    for (int i = 2; i < final_len; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    /* Sum contributions of bits set to 1. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < final_len; ++i) {
        if (final_bits[final_len - 1 - i]) { /* LSB corresponds to fib[0] */
            result += fib[i];
        }
    }
    return result;
}
