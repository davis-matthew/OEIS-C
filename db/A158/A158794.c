/*
 * A158794  Multiples of 4 which are not the sum of seven nonnegative cubes.
 * https://oeis.org/A158794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  bref, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A158794: multiples of 4 that are NOT the sum
 * of seven non‑negative integer cubes.
 * The sequence is finite; all multiples of 4 greater than 452 are representable
 * (Elkies 2010).  The function returns the nth term (n=1 is the first term)
 * or 0 if n exceeds the number of terms. */
#include <stdbool.h>
#include <stdint.h>

/* Maximum value to test (largest multiple of 4 below 454). */
#define MAX_TEST 452

/* Maximum possible number of terms: all multiples of 4 up to MAX_TEST.
 * 452/4 = 113, so we reserve space for 113 entries. */
#define MAX_TERMS 113

static seq_t seq_terms[MAX_TERMS];
static idx_t seq_len = 0;
static bool seq_init_done = false;

/* One‑time initialization: find which multiples of 4 up to MAX_TEST
 * are not representable as a sum of seven non‑negative cubes. */
static void init_sequence(void)
{
    if (seq_init_done)
        return;
    seq_init_done = true;

    /* Boolean table for representable values up to MAX_TEST. */
    unsigned char representable[MAX_TEST + 1] = {0};

    /* Precompute cubes of 0..7.  7^3 = 343 < MAX_TEST, so all values
     * fit in 32 bits and are represented in unsigned long long to avoid
     * any overflow when summing. */
    unsigned long long cubes[8];
    for (unsigned long long i = 0; i <= 7; ++i)
        cubes[i] = i * i * i;

    /* Enumerate all 7‑tuples of cube roots 0..7 and mark representable sums. */
    for (unsigned a = 0; a <= 7; ++a)
        for (unsigned b = 0; b <= 7; ++b)
            for (unsigned c = 0; c <= 7; ++c)
                for (unsigned d = 0; d <= 7; ++d)
                    for (unsigned e = 0; e <= 7; ++e)
                        for (unsigned f = 0; f <= 7; ++f)
                            for (unsigned g = 0; g <= 7; ++g) {
                                unsigned long long s =
                                    cubes[a] + cubes[b] + cubes[c] +
                                    cubes[d] + cubes[e] + cubes[f] + cubes[g];
                                if (s <= MAX_TEST)
                                    representable[(unsigned)s] = 1;
                            }

    /* Collect multiples of 4 that are NOT representable. */
    for (unsigned m = 4; m <= MAX_TEST; m += 4)
        if (!representable[m] && seq_len < MAX_TERMS)
            seq_terms[seq_len++] = (seq_t)m;
}

seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0)          /* offset is 1; below that return 0 */
        return (seq_t)0;
    if (n <= seq_len)
        return seq_terms[n - 1];
    return (seq_t)0;     /* beyond the finite sequence */
}
