/*
 * A085139  a(n) = Sum_{i=0..n-1} (1 + (-1)^(n-1-i))/2 * Sum_{j=0..i}
 *          a(j)*a(i-j) for n > 0, with a(0) = 1.
 * https://oeis.org/A085139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{i=0..n‑1} ((1+(-1)^(n‑1‑i))/2) · Σ_{j=0..i} a(j)·a(i‑j)
 * Initial value: a(0) = 1
 * The implementation pre‑computes the sequence lazily.  For a given
 * n it expands the stored tables up to that index.  conv[i] = Σ_{j=0..i}
 * a(j)·a(i‑j) and two cumulative sums cum[0] / cum[1] keep
 * separatly the sums of conv‑terms with even / odd indices.  Then
 * a(n) is simply cum[(n‑1)&1].
 */

#include <stddef.h>
#include <stdlib.h>

/* External definitions of seq_t and idx_t are provided by the build */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/*   internal helper to grow a dynamic array of seq_t           */
/* ------------------------------------------------------------ */
static void ensure_cap(idx_t needed, seq_t **p, size_t *cap)
{
    size_t ncap = *cap;
    if (ncap < (size_t)needed)
    {
        ncap = ncap ? ncap << 1 : 1;
        while (ncap < (size_t)needed) ncap <<= 1;
        *p = (seq_t *)realloc(*p, ncap * sizeof(seq_t));
        *cap = ncap;
    }
}

/* ------------------------------------------------------------ */
/*                         main routine                          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------------- */
    /* tables of a‑values and convolution sums */
    static seq_t *seq  = NULL;
    static size_t seq_cap = 0;
    static seq_t *conv = NULL;
    static size_t conv_cap = 0;
    /* cumulative sums of conv for even / odd indices */
    static seq_t cum[2] = {0, 0};
    /* index of the last computed term */
    static idx_t last = (idx_t)-1;            /* -1 i.e. nothing computed */

    /* initialise on first call ----------------------------------------------- */
    if (last == (idx_t)-1)
    {
        /* allocate capacity for the zeroth term */
        ensure_cap(1, &seq,  &seq_cap);
        ensure_cap(1, &conv, &conv_cap);

        seq[0]  = (seq_t)1;          /* a(0)   */
        conv[0] = seq[0] * seq[0];   /* conv[0] = a(0)*a(0)  */
        cum[0]  = conv[0];           /* even index 0 */
        cum[1]  = 0;
        last = 0;
    }

    /* grow tables if necessary --------------------------------------------- */
    if (n >= seq_cap)  ensure_cap((idx_t)n + 1, &seq,  &seq_cap);
    if (n >= conv_cap) ensure_cap((idx_t)n + 1, &conv, &conv_cap);

    /* compute up to n -------------------------------------------------------- */
    while (last < n)
    {
        idx_t next = last + 1;        /* index to compute */

        /* a(next) is the sum of conv[i] for i < next having parity = parity(next-1) */
        seq[next] = cum[(next - 1) & 1U];

        /* compute conv[next] = Σ_{j=0..next} a(j)·a(next‑j) */
        seq_t s = 0;
        for (idx_t j = 0; j <= next; ++j)
        {
            s += seq[j] * seq[next - j];
        }
        conv[next] = s;

        /* update cumulative sum of this parity */
        cum[next & 1U] += conv[next];

        last = next;
    }

    /* return requested value */
    return seq[n];
}
