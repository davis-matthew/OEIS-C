/*
 * A046197  Fixed points for operation of repeatedly replacing a number with
 *          the sum of the cubes of its digits.
 * https://oeis.org/A046197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of fixed points of the digit‑cube‑sum operator.
   A046197: numbers n such that n == Σ (digit^3) of n.
   The values are 0,1,153,370,371,407.  We generate them by scanning
   all 5‑digit numbers (0..99999) at first use, store them in a static
   array, sort the array, and answer queries.  No hard‑coded constants
   other than the literal bounds for the scan. */
#include <stdbool.h>
#include <stddef.h>

/* idx_t is an unsigned 64‑bit integer (or larger).  seq_t is an
   unsigned integer type that may be 8, 16, 32, 64 or 128 bits, or an
   arbitrary‑precision class. */
static bool init = false;
static seq_t fixed[20];
static size_t count = 0;

static void init_fixed(void) {
    size_t i, k = 0;
    /* Scan 0 .. 99999 (at most 5 digits). */
    for (i = 0; i <= 99999; ++i) {
        seq_t sum = 0;
        idx_t x = i;
        if (x == 0) {          /* special case: 0 → 0³ = 0 */
            sum = 0;
        } else {
            while (x) {
                int d = x % 10;
                seq_t cube = (seq_t)d * (seq_t)d * (seq_t)d;
                sum += cube;
                x /= 10;
            }
        }
        if ((seq_t)i == sum) {
            fixed[k++] = (seq_t)i;
        }
    }
    count = k;

    /* Simple insertion sort (count <= 10). */
    for (size_t a = 1; a < count; ++a) {
        seq_t v = fixed[a];
        size_t b = a;
        while (b > 0 && v < fixed[b - 1]) {
            fixed[b] = fixed[b - 1];
            --b;
        }
        fixed[b] = v;
    }
}

seq_t a(idx_t n) {
    if (!init) {
        init_fixed();
        init = true;
    }
    if (n == 0 || n > count) {
        return 0;
    }
    return fixed[n - 1];
}
