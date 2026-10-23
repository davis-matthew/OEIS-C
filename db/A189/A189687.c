/*
 * A189687  Fixed point of the morphism 0->011, 1->01.
 * https://oeis.org/A189687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189687: Fixed point of the morphism 0->011, 1->01.
   a(n) is the n‑th (1‑based) bit of the infinite word obtained by iterating
   the substitution 0→011, 1→01 starting from the single character "0".
   The algorithm keeps a dynamically growing buffer containing the word
   up to the largest requested index.  For each request n we expand the
   buffer until it contains at least n bits, then return buffer[n‑1]. */

#include <stdlib.h>   /* malloc, free, exit */

/* The build supplies the integer types seq_t and idx_t.  We just use them. */

/* Dynamically stored prefix of the fixed point. */
static seq_t *wordbuf = NULL;   /* array of bits (0 or 1) */
static size_t wordlen = 0;      /* number of bits stored */

/* Ensure that wordbuf contains at least n bits (n >= 1). */
static void extend_to(idx_t n)
{
    /* Initialize with the first word "0" if we haven't yet. */
    if (wordbuf == NULL) {
        wordbuf = (seq_t *)malloc(sizeof(seq_t));
        if (!wordbuf) exit(1);          /* out of memory */
        wordbuf[0] = (seq_t)0;
        wordlen = 1;
    }

    /* Grow until we have enough bits. */
    while (wordlen < (size_t)n) {
        size_t oldlen = wordlen;
        /* Example maximum growth: every 0 turns into three characters,
           every 1 into two.  We use a safe over‑estimate. */
        size_t estlen = oldlen * 3;
        seq_t *newbuf = (seq_t *)malloc(estlen * sizeof(seq_t));
        if (!newbuf) exit(1);          /* out of memory */

        size_t pos = 0;
        for (size_t i = 0; i < oldlen; ++i) {
            if (wordbuf[i] == (seq_t)0) {
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)1;
                newbuf[pos++] = (seq_t)1;
            } else {
                newbuf[pos++] = (seq_t)0;
                newbuf[pos++] = (seq_t)1;
            }
        }

        free(wordbuf);
        wordbuf = newbuf;
        wordlen = pos;
    }
}

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined below offset */

    extend_to(n);                        /* guarantees wordlen >= n */
    return wordbuf[(size_t)(n - 1)];      /* 1‑based index */
}
