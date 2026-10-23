/*
 * A143667  Digits of the infinite Fibonacci word A003849 grouped 2 by 2 and
 *          interpreted as a binary value.
 * https://oeis.org/A143667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = value of pair of bits (b(2n‑2), b(2n‑1)) from the
 *            infinite Fibonacci word A003849; mapping 00→0, 01→1, 10→2. The
 *            word never contains the pair 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pair_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pair_value.h"

static uint8_t *fib_word = NULL;   /* bits of the Fibonacci word */
static size_t fib_len   = 0;       /* current length of fib_word */

/* Grow fib_word one substitution step: 0→01, 1→0.  The new length is
   cnt0*2 + cnt1, where cnt0 and cnt1 are counts of zeros and ones
   in the current word. */
static void expand_word(void)
{
    size_t cnt0 = 0, cnt1 = 0;
    for (size_t i = 0; i < fib_len; ++i) {
        if (fib_word[i] == 0)
            ++cnt0;
        else
            ++cnt1;
    }
    size_t new_len = cnt0 * 2 + cnt1;

    uint8_t *new_word = (uint8_t*)malloc(new_len);
    size_t pos = 0;
    for (size_t i = 0; i < fib_len; ++i) {
        if (fib_word[i] == 0) {
            new_word[pos++] = 0;
            new_word[pos++] = 1;
        } else {
            new_word[pos++] = 0;
        }
    }

    free(fib_word);
    fib_word = new_word;
    fib_len  = new_len;
}

/* Ensure fib_word contains at least `need` bits.  The function
   grows the word as needed, starting from the single bit '0'. */
static void ensure_bits(idx_t need)
{
    if (fib_word == NULL) {
        fib_word = (uint8_t*)malloc(1);
        fib_word[0] = 0;
        fib_len = 1;
    }

    while (fib_len < (size_t)need)
        expand_word();
}

/* Return the n-th term (1‑indexed) of A143667. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n == 0, but harmless */

    idx_t needed = 2 * n;     /* number of bits required */
    ensure_bits(needed);

    size_t idx = 2 * (size_t)(n - 1);
    uint8_t a_bit = fib_word[idx];
    uint8_t b_bit = fib_word[idx + 1];
    return pair_value(a_bit, b_bit);
}
