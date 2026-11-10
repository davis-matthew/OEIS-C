/*
 * A207325  Primes p which divide A003499((p-1)/2)+6 and do not divide
 *          A003499(n) + 6 where n < (p-1)/2.
 * https://oeis.org/A207325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd prime p with A003499((p-1)/2)+6 == 0 (mod p) and
 *            A003499(m)+6 != 0 (mod p) for all m < (p-1)/2, where A003499(m)
 *            = 6*A003499(m-1) - A003499(m-2), A003499(0)=2, A003499(1)=6;
 *            found by search over primes, computing the recurrence mod p
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A207325_isprime(uint64_t x)
{
	if (x < 2) return false;
	for (uint64_t d = 2; d * d <= x; d++)
		if (x % d == 0) return false;
	return true;
}

static bool A207325_ok(uint64_t p)
{
	uint64_t h = (p - 1) / 2;
	uint64_t target = (p - (6 % p)) % p;
	uint64_t prev = 2 % p, curr = 6 % p;
	for (uint64_t m = 0; m <= h; m++) {
		uint64_t v;
		if (m == 0) v = prev;
		else if (m == 1) v = curr;
		else {
			uint64_t nx = (6 * curr + p - prev) % p;
			prev = curr;
			curr = nx;
			v = curr;
		}
		if (v == target) return m == h;
	}
	return false;
}

seq_t a(idx_t n)
{
	uint64_t count = 0;
	uint64_t p = 1;
	while (count < (uint64_t)n) {
		p += 2;
		if (!A207325_isprime(p)) continue;
		if (A207325_ok(p)) count++;
	}
	return (seq_t)p;
}
