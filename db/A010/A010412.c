/*
 * A010412  Squares mod 51.
 * https://oeis.org/A010412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth element of the sorted list of distinct quadratic
 *            residues modulo 51. Equivalently, it is the nth smallest number
 *            in {k^2 mod 51 : k integers}
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compilation: must be placed in a .c or .cpp file.  seq_t and idx_t are
   provided by the build system. */

#include <stdlib.h>
#include <stdbool.h>

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* initialize the array of distinct residues modulo 51 */
static void init_residues(void)
{
    static seq_t values[51];
    static size_t count = 0;

    bool seen[51] = {false};
    count = 0;

    for (idx_t k = 0; k < 51; ++k) {
        seq_t r = ((seq_t)k * k) % (seq_t)51;
        if (!seen[(size_t)r]) {
            seen[(size_t)r] = true;
            values[count++] = r;
        }
    }
    qsort(values, count, sizeof(seq_t), cmp_seq);

    /* store the computed array in the static area */
    static seq_t *storage = NULL;
    if (!storage) {
        storage = (seq_t *)values; /* values is static, safe to use directly */
    }
}

/* external function required by the task */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t res[51];
    static size_t len = 0;

    if (!initialized) {
        /* Compute once */
        bool seen[51] = {false};
        len = 0;
        for (idx_t k = 0; k < 51; ++k) {
            seq_t r = ((seq_t)k * k) % (seq_t)51;
            if (!seen[(size_t)r]) {
                seen[(size_t)r] = true;
                res[len++] = r;
            }
        }
        qsort(res, len, sizeof(seq_t), cmp_seq);
        initialized = true;
    }

    /* The sequence is defined for n >= 1.  If n exceeds the number of
       distinct residues, we wrap around modulo the count. */
    if (n == 0) return 0;
    size_t idx = (size_t)(n - 1) % len;
    return res[idx];
}
