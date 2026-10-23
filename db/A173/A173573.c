/*
 * A173573  3-Factorions: equal to the sum of the triple factorials of their
 *          digits in base 10.
 * https://oeis.org/A173573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   a(n) = number whose digits' triple factorial sum equals the
 *            number. Sequence is finite: 1 ≤ n ≤ 8 (for 4‑digit numbers) –
 *            computed by brute‑force search up to 9999
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static const seq_t _digit_tf[10] = {1,1,2,3,4,10,18,28,80,162};

static seq_t _seq[10000];
static size_t _seq_len = 0;
static bool _seq_init = false;

static void _init_seq(void)
{
    if (_seq_init)
        return;
    _seq_init = true;

    for (idx_t num = 1; num <= 9999; ++num) {
        seq_t sum = 0;
        idx_t tmp = num;
        while (tmp) {
            unsigned digit = (unsigned)(tmp % 10);
            sum += _digit_tf[digit];
            tmp /= 10;
        }
        if (sum == (seq_t)num) {
            _seq[_seq_len++] = (seq_t)num;
        }
    }
}

seq_t a(idx_t n)
{
    _init_seq();
    if (n == 0 || n > _seq_len)
        return (seq_t)0;
    return _seq[n-1];
}
