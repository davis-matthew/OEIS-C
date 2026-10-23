/*
 * A018574  Divisors of 654.
 * https://oeis.org/A018574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 654 in ascending order.
   The function returns the nth smallest divisor of 654.
   Out‑of‑range n return 0. */
#include <stddef.h>
#include <stdbool.h>

static seq_t divisors[8];
static bool ready = false;

static void compute_divisors(void)
{
    /* Factors of 654 are 2, 3 and 109, each to the first power. */
    seq_t *arr = divisors;
    int idx = 0;
    for (int i = 0; i < 2; ++i) {
        seq_t d1 = (i ? (seq_t)2 : (seq_t)1);
        for (int j = 0; j < 2; ++j) {
            seq_t d2 = (j ? (seq_t)3 : (seq_t)1);
            for (int k = 0; k < 2; ++k) {
                seq_t d = d1 * d2;
                if (k) d *= (seq_t)109;
                arr[idx++] = d;
            }
        }
    }
    /* simple bubble sort to order the eight divisors */
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                seq_t tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) compute_divisors();
    if (n == 0) return (seq_t)0;   /* undefined below offset */
    size_t idx = (size_t)(n - 1);
    if (idx < 8) return divisors[idx];
    return (seq_t)0; /* out of range */
}
