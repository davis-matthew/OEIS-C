/*
 * A156596  Infinite Fibonacci word fractal sequence.
 * https://oeis.org/A156596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) is obtained by mapping consecutive non‑overlapping pairs of
 *            bits of the Fibonacci word with the rule 00 → 12 , 01 → 10 , 10
 *            → 02 The first digit of the pair is returned for odd n, the
 *            second for even n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers used for memory allocation and size_t. */
#include <stdlib.h>

/* Fibtenary bits of the Fibonacci word: 0 → 01, 1 → 0. */
static unsigned char *fib_bits = NULL;   /* buffer of bits (0 or 1) */
static size_t          fib_len   = 0;    /* current number of bits stored */
static size_t          fib_cap   = 0;    /* current buffer capacity   */

/* Ensure that at least 'need' bits are available in fib_bits. */
static void
ensure_fib_bits(size_t need)
{
    /* Initialise on first call. */
    if (fib_len == 0) {
        fib_bits = (unsigned char*)malloc(1);
        if (!fib_bits) abort();
        fib_bits[0] = 0;
        fib_len = 1;
        fib_cap = 1;
    }

    /* Expand until the buffer contains at least 'need' bits. */
    while (fib_len < need) {
        size_t old_len = fib_len;

        /* Count zeros in the current word to compute the new length after expansion. */
        size_t zero_count = 0;
        for (size_t i = 0; i < old_len; ++i)
            if (fib_bits[i] == 0) ++zero_count;
        size_t new_len = old_len + zero_count;

        /* Grow the buffer if necessary. */
        size_t new_cap = fib_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < new_len)
            new_cap <<= 1;          /* double until big enough */

        unsigned char *new_bits = (unsigned char*)malloc(new_cap);
        if (!new_bits) abort();

        /* Apply the morphism to each old bit. */
        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i) {
            if (fib_bits[i] == 0) {
                new_bits[pos++] = 0; /* 0 → 01    */
                new_bits[pos++] = 1;
            } else {
                new_bits[pos++] = 0; /* 1 → 0     */
            }
        }
        /* Replace the old buffer. */
        free(fib_bits);
        fib_bits = new_bits;
        fib_len  = pos;
        fib_cap  = new_cap;
    }
}

/* Mapping table for the pair → two output digits. */
static const unsigned char map_table[4][2] = {
    {1, 2},   /* 00 → 12 */
    {1, 0},   /* 01 → 10 */
    {0, 2},   /* 10 → 02 */
    {0, 0}    /* 11 never occurs */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* the sequence is 1‑based */

    /* Pair index k = ceil(n/2).  k = (n + 1) / 2. */
    idx_t k   = (n + 1) / 2;
    size_t need = 2 * (size_t)k;           /* number of bits needed up to 2k */
    ensure_fib_bits(need);

    unsigned char b1 = fib_bits[2 * k - 2];  /* zero‑based indices */
    unsigned char b2 = fib_bits[2 * k - 1];

    unsigned int key = ((unsigned)b1 << 1) | b2;            /* 00→0, 01→1, 10→2, 11→3 */
    unsigned char first  = map_table[key][0];
    unsigned char second = map_table[key][1];

    return (n & 1) ? (seq_t)first : (seq_t)second;
}
