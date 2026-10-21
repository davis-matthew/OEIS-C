/*
 * A227339  Fixed point of the morphism 1 -> 131, 2 -> 312, 3 -> 2.
 * https://oeis.org/A227339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fixed point of the morphism 1 → 131, 2 → 312, 3 → 2
 *            (indices start at 1)
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static idx_t lengths[64][3];          /* lengths[d][i] = |φ^d(i+1)|      */
static size_t max_depth_computed = 0; /* highest depth computed so far */

/* images of each symbol (1‑based indices) */
static const seq_t images[3][3] = {
    {1, 3, 1},      /* φ(1) = 131 */
    {3, 1, 2},      /* φ(2) = 312 */
    {2, 0, 0}       /* φ(3) = 2   (trailing zeros unused) */
};
static const int img_len[3] = {3, 3, 1};

/* compute the lengths array for the next depth */
static void compute_next_depth(void)
{
    size_t d = max_depth_computed + 1;
    for (int s = 0; s < 3; ++s) {
        idx_t sum = 0;
        for (int k = 0; k < img_len[s]; ++k) {
            seq_t sub = images[s][k];
            idx_t sublen = lengths[d - 1][(int)(sub - 1)];
            if (sum > UINT64_MAX - sublen)
                sum = UINT64_MAX;       /* prevent overflow */
            else
                sum += sublen;
        }
        lengths[d][s] = sum;
    }
    max_depth_computed = d;
}

/* recursively find the symbol at position `pos` (1‑based) in the
   image of `symbol` after `depth` iterations of the morphism. */
static seq_t find_symbol(size_t depth, idx_t pos, seq_t symbol)
{
    if (depth == 0)
        return symbol;

    const seq_t *im = images[(int)(symbol - 1)];
    int len = img_len[(int)(symbol - 1)];
    idx_t offset = 0;

    for (int i = 0; i < len; ++i) {
        seq_t sub = im[i];
        idx_t block_len = lengths[depth - 1][(int)(sub - 1)];
        idx_t diff = pos - offset;          /* pos > offset always here */
        if (diff <= block_len)              /* inside this block */
            return find_symbol(depth - 1, diff, sub);
        offset += block_len;
    }
    return 0; /* unreachable */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    /* initialise base case on first call */
    if (max_depth_computed == 0) {
        for (int i = 0; i < 3; ++i)
            lengths[0][i] = 1;
        max_depth_computed = 0;
    }

    size_t depth = max_depth_computed;
    while (n > lengths[depth][0]) {   /* need deeper level */
        compute_next_depth();
        ++depth;
    }
    return find_symbol(depth, n, (seq_t)1);
}
