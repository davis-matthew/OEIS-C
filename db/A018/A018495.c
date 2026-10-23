/*
 * A018495  Divisors of 513.
 * https://oeis.org/A018495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 513 (3^3·19) in increasing order.
   a(1)=1, a(2)=3, a(3)=9, a(4)=19, a(5)=27, a(6)=57, a(7)=171, a(8)=513. */

#include <stddef.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t divisors[8];
static size_t divisor_count = 0;

/* Compute all divisors of 513 and sort them ascending.  Must be
   called at most once; subsequent calls return immediately. */
static void init_divisors(void)
{
    if (initialized) return;

    const seq_t p1 = (seq_t)3;
    const seq_t p2 = (seq_t)19;

    /* 3^a with a = 0..3, 19^b with b = 0..1 */
    for (int a = 0; a <= 3; ++a) {
        seq_t pow3 = (seq_t)1;
        for (int i = 0; i < a; ++i) pow3 *= p1;

        for (int b = 0; b <= 1; ++b) {
            seq_t pow19 = (seq_t)1;
            for (int j = 0; j < b; ++j) pow19 *= p2;

            divisors[divisor_count++] = pow3 * pow19;
        }
    }

    /* simple bubble sort – only eight elements */
    for (size_t i = 0; i < divisor_count; ++i) {
        for (size_t j = i + 1; j < divisor_count; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    init_divisors();

    if ((idx_t)divisor_count < n) return (seq_t)0;  /* beyond the fixed 8 terms */
    return divisors[(size_t)n - 1];
}
