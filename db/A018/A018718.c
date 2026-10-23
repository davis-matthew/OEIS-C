/*
 * A018718  Divisors of 912.
 * https://oeis.org/A018718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the list of positive divisors of 912 in increasing order.
   The prime factorisation of 912 is 2⁴ · 3 · 19, so there are
   (4+1)(1+1)(1+1) = 20 divisors.  The n‑th term a(n) is the n‑th smallest
   among these divisors (a(1) = 1, a(20) = 912). */
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort: ascending order. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a > b) - (a < b);
}

/* Cached array of all divisors of 912.  They are generated once and
   sorted lazily. */
static seq_t divisors[20];
static bool initialized = false;

static void init_divisors(void)
{
    if (initialized)
        return;

    idx_t pos = 0;
    /* 2⁴ · 3¹ · 19¹  */
    for (idx_t e2 = 0; e2 <= 4; ++e2) {
        seq_t v2 = (seq_t)1 << e2;             /* 2^e2 */
        for (idx_t e3 = 0; e3 <= 1; ++e3) {
            seq_t v3 = (e3 == 0) ? (seq_t)1 : (seq_t)3;
            for (idx_t e19 = 0; e19 <= 1; ++e19) {
                seq_t v19 = (e19 == 0) ? (seq_t)1 : (seq_t)19;
                divisors[pos++] = v2 * v3 * v19;
            }
        }
    }

    /* Sort ascending. */
    qsort(divisors, 20, sizeof(seq_t), cmp_seq_t);
    initialized = true;
}

/* Public interface.  a(1) is the smallest divisor, a(20) the largest. */
seq_t a(idx_t n)
{
    if (n == 0 || n > 20)
        return (seq_t)0;   /* undefined for out‑of‑range indices */
    init_divisors();
    return divisors[n - 1];
}
