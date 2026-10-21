/*
 * A202844  Number of secondary structures of size n having no stacks of
 *          length 3.
 * https://oeis.org/A202844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts secondary structures of size n with no stacks of
 *            length 3. It satisfies the quadratic equation (z^2 - z^6 + z^8)
 *            G^2 + (z - 1 - z^2 - z^7 + z^9 + 2 z^6 - 2 z^8) G + (1 - z^6 +
 *            z^8) = 0 where G(z)=∑ a(n) z^n. From this we obtain the
 *            recurrence a(n) = a(n-1) - a(n-2) + 2 a(n-6) - a(n-7)...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Allocate arrays for the sequence and its autocorrelation (convolution). */
    size_t len = (size_t)n + 1;               /* use signed+size_t safely */
    seq_t *arr = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *conv = (seq_t *)calloc(len, sizeof(seq_t));
    if (!arr || !conv) {                      /* allocation failed */
        return (seq_t)0;                      /* caller must handle error */
    }

    /* Initial values (offset 0). */
    if (n >= 0) arr[0] = (seq_t)1;
    if (n >= 1) arr[1] = (seq_t)1;
    if (n >= 2) arr[2] = (seq_t)1;
    if (n >= 3) arr[3] = (seq_t)2;
    if (n >= 4) arr[4] = (seq_t)4;
    if (n >= 5) arr[5] = (seq_t)8;

    /* Pre‑compute convolution values for the base cases. */
    size_t i;
    for (i = 0; i <= (size_t)(n < 5 ? n : 5); ++i) {
        conv[i] = (seq_t)0;
        idx_t k;
        for (k = 0; k <= (idx_t)i; ++k)
            conv[i] += arr[(size_t)k] * arr[i - (size_t)k];
    }

    /* Recurrence for n ≥ 6. */
    for (i = 6; i <= (size_t)n; ++i) {
        /* Linear part: a(i-1) - a(i-2) + 2 a(i-6) - a(i-7) - 2 a(i-8) + a(i-9) */
        seq_t term = arr[i - 1] - arr[i - 2];
        if (i >= 6) term += (seq_t)2 * arr[i - 6];
        if (i >= 7) term -= arr[i - 7];
        if (i >= 8) term -= (seq_t)2 * arr[i - 8];
        if (i >= 9) term += arr[i - 9];

        /* Convolution part */
        if (i >= 2) term += conv[i - 2];
        if (i >= 6) term -= conv[i - 6];
        if (i >= 8) term += conv[i - 8];

        /* Constant term C(n) */
        if (i == 6) term -= (seq_t)1;
        else if (i == 8) term += (seq_t)1;

        arr[i] = term;

        /* Compute conv[i] = Σ_{j=0}^{i} a(j) * a(i-j) */
        conv[i] = (seq_t)0;
        idx_t j;
        for (j = 0; j <= (idx_t)i; ++j)
            conv[i] += arr[(size_t)j] * arr[i - (size_t)j];
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    free(conv);
    return result;
}
