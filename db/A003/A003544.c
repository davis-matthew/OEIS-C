/*
 * A003544  Divisors of 2^38 - 1.
 * https://oeis.org/A003544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^38 - 1 (found by trial
 *            division up to sqrt(2^38 - 1))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A003544_divs[64];
static int A003544_cnt = 0;

static void A003544_init(void)
{
	uint64_t N = (((uint64_t)1) << 38) - 1;
	uint64_t small[32];
	int ns = 0, i;
	uint64_t d;
	for (d = 1; d * d <= N; d++)
		if (N % d == 0)
			small[ns++] = d;
	A003544_cnt = 0;
	for (i = 0; i < ns; i++)
		A003544_divs[A003544_cnt++] = small[i];
	for (i = ns - 1; i >= 0; i--) {
		uint64_t q = N / small[i];
		if (q != small[i])
			A003544_divs[A003544_cnt++] = q;
	}
}

seq_t a(idx_t n)
{
	if (A003544_cnt == 0)
		A003544_init();
	if (n < 1 || n > (idx_t)A003544_cnt)
		return 0;
	return (seq_t)A003544_divs[n - 1];
}
