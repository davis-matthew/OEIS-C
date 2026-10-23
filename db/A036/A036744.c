/*
 * A036744  Penholodigital squares: squares containing each of the digits 1..9
 *          exactly once.
 * https://oeis.org/A036744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = A071519(n)^2 A071519(n) is the n-th integer whose square
 *            is a 9‑digit pandigital (digits 1..9 each occurring exactly
 *            once). The sequence A036744 consists of the squares themselves.
 *            The implementation pre‑computes all such squares in the interval
 *            [11112,31427] (the only possible roots of a...
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *sqr_array = NULL; /* array of penholodigital squares */
static idx_t sqr_count = 0;     /* number of squares stored */
static size_t sqr_cap = 0;      /* current capacity of the array */

/* Build the array of all penholodigital squares only once. */
static void build_squares(void) {
    if (sqr_array != NULL) return;     /* already built */

    /* initial capacity (will grow as needed) */
    sqr_cap = 16;
    sqr_array = (seq_t*)malloc(sqr_cap * sizeof(seq_t));
    sqr_count = 0;

    /* iterate over all possible roots */
    for (idx_t x = 11112; x <= 31427; ++x) {
        if ((x % 3) != 0) continue;   /* squares must be divisible by 3 */

        seq_t root = (seq_t)x;
        seq_t sq  = root * root;      /* the candidate square */

        unsigned char cnt[10] = {0};
        seq_t t = sq;
        bool ok = true;

        /* Extract 9 decimal digits */
        for (int i = 0; i < 9; ++i) {
            unsigned d = (unsigned)(t % 10);
            if (d > 9) { ok = false; break; } /* should not happen */
            cnt[d]++;
            if (cnt[d] > 1) { ok = false; break; }
            t /= 10;
        }
        if (!ok || t != 0) continue;      /* wrong digit count or duplicate */

        if (cnt[0] != 0) continue;        /* digit 0 not allowed */
        for (int d = 1; d <= 9; ++d) {
            if (cnt[d] != 1) { ok = false; break; }
        }
        if (!ok) continue;                 /* failed digit test */

        /* store the square, growing the array if necessary */
        if (sqr_count == sqr_cap) {
            sqr_cap <<= 1;
            sqr_array = (seq_t*)realloc(sqr_array, sqr_cap * sizeof(seq_t));
        }
        sqr_array[sqr_count++] = sq;
    }
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n) {
    build_squares();
    if (n == 0 || n > sqr_count) return 0;  /* undefined for out‑of‑range */
    return sqr_array[n - 1];
}
