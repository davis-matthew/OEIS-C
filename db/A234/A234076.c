/*
 * A234076  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 10.
 * https://oeis.org/A234076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 22*a(n-2) - 56*a(n-3) - 120*a(n-4) + 32*a(n-5)
 *            + 64*a(n-6) (empirical), a(1)=96, a(2)=476, a(3)=2352,
 *            a(4)=12152, a(5)=63136, a(6)=335536
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *seq = NULL;
static idx_t seq_cap = 0;
static idx_t computed_max = 0;

/* Ensure the array has space for indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n < seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    /* Initialise any new elements to zero. */
    for (idx_t i = seq_cap; i < new_cap; ++i)
        seq[i] = 0;
    seq_cap = new_cap;
}

/* Initialise the first six known values. */
static void init_base(void)
{
    if (!seq)
        ensure_capacity(6);
    if (computed_max < 6) {
        seq[1] = (seq_t)96;
        seq[2] = (seq_t)476;
        seq[3] = (seq_t)2352;
        seq[4] = (seq_t)12152;
        seq[5] = (seq_t)63136;
        seq[6] = (seq_t)335536;
        computed_max = 6;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    init_base();
    if (n <= computed_max)
        return seq[n];

    ensure_capacity(n);
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = (seq_t)4 * seq[i-1] + (seq_t)22 * seq[i-2]
                    + (seq_t)32 * seq[i-5] + (seq_t)64 * seq[i-6];
        seq_t sub = (seq_t)56 * seq[i-3] + (seq_t)120 * seq[i-4];
        seq[i] = pos - sub;
    }
    computed_max = n;
    return seq[n];
}
