/*
 * A024601  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (odd natural numbers), t = A023533.
 * https://oeis.org/A024601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} (2k-1) * A023533(n-k+1)
 * Range:     u8 n <= 165, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of tetrahedral numbers (idx_t). */
static idx_t *tetra = NULL;        /* array of tetrahedral numbers */
static size_t tetra_len = 0;       /* number of elements in the cache */
static idx_t max_precomputed = 0;  /* largest value up to which the cache is filled */

/* Ensure that tetrahedral numbers up to 'upper' are stored in the cache. */
static void ensure_tetra_upto(idx_t upper) {
    if (upper <= max_precomputed)
        return;                     /* already have enough */

    /* current last tetrahedral number and next index to compute */
    idx_t cur_tetra = (tetra_len > 0) ? tetra[tetra_len - 1] : 0;
    idx_t next_m = (idx_t)(tetra_len + 1);  /* next m to compute */

    while (1) {
        /* triangular number T_m = m*(m+1)/2 */
        idx_t tri = (next_m * (next_m + 1)) / 2;
        idx_t new_tetra = cur_tetra + tri;
        if (new_tetra > upper)
            break;                    /* no more needed */

        /* enlarge the cache */
        idx_t *tmp = (idx_t *)realloc(tetra, (tetra_len + 1) * sizeof(idx_t));
        if (!tmp)
            exit(EXIT_FAILURE);       /* allocation failure */

        tetra = tmp;
        tetra[tetra_len] = new_tetra;
        tetra_len++;
        cur_tetra = new_tetra;
        next_m++;
    }
    max_precomputed = upper;
}

/* Compute a(n). */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;                    /* undefined for n < 1, return 0 */

    /* Get all tetrahedral numbers up to n. */
    ensure_tetra_upto(n);

    /* k_max = floor((n+1)/2) and the lower bound for contributing tetrahedral numbers */
    idx_t k_max = (n + 1) / 2;
    idx_t L = n - k_max + 1;         /* = n - floor((n+1)/2) + 1 */

    seq_t sum = 0;
    for (size_t i = 0; i < tetra_len; ++i) {
        idx_t t = tetra[i];
        if (t < L)
            continue;
        if (t > n)
            break;                   /* remaining t are larger */
        /* contribution: (2*n - 2*t + 1) */
        sum += (seq_t)(2 * n - 2 * t + 1);
    }
    return sum;
}
