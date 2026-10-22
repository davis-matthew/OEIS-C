/*
 * A005042  Primes formed by the initial digits of the decimal expansion of
 *          Pi.
 * https://oeis.org/A005042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10^(A060421(n)-1)*A000796). Search: take the
 *            prefixes of Pi's decimal digits in order of length and test each
 *            for primality (trial division by small primes, then Miller-Rabin
 *            over seq_t); a(n) is the n-th prime prefix
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   addmod_s, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/addmod_s.h"

static const char pi_digits[] =
	"3141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067";

static const unsigned small_primes[] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};
#define NSMALL (sizeof(small_primes) / sizeof(small_primes[0]))

static seq_t mulmod_s(seq_t x, seq_t y, seq_t m) {
	if (!seq_is_fixed_width()) return (x * y) % m;
	seq_t r = 0;
	x %= m;
	while (y > 0) {
		if ((y & 1) != 0) r = addmod_s(r, x, m);
		x = addmod_s(x, x, m);
		y >>= 1;
	}
	return r;
}

static seq_t powmod_s(seq_t b, seq_t e, seq_t m) {
	seq_t r = 1 % m;
	b %= m;
	while (e > 0) {
		if ((e & 1) != 0) r = mulmod_s(r, b, m);
		b = mulmod_s(b, b, m);
		e >>= 1;
	}
	return r;
}

static bool is_prime_s(seq_t m) {
	if (m < 2) return false;
	for (size_t i = 0; i < NSMALL; i++) {
		if (m == (seq_t)small_primes[i]) return true;
		if (m % small_primes[i] == 0) return false;
	}
	seq_t d = m - 1;
	unsigned s = 0;
	while ((d & 1) == 0) {
		d >>= 1;
		s++;
	}
	for (size_t i = 0; i < NSMALL; i++) {
		seq_t x = powmod_s((seq_t)small_primes[i], d, m);
		if (x == 1 || x == m - 1) continue;
		bool comp = true;
		for (unsigned r = 1; r < s; r++) {
			x = mulmod_s(x, x, m);
			if (x == m - 1) {
				comp = false;
				break;
			}
		}
		if (comp) return false;
	}
	return true;
}

seq_t a(idx_t n) {
	idx_t count = 0;
	size_t len = strlen(pi_digits);
	bool fixed = seq_is_fixed_width();
	seq_t maxv = (seq_t)0 - 1;
	seq_t num = 0;

	for (size_t i = 0; i < len; i++) {
		if (fixed && num > (maxv - 9) / 10) break;
		num = num * 10 + (seq_t)(pi_digits[i] - '0');
		if (is_prime_s(num)) {
			count++;
			if (count == n) return num;
		}
	}
	return 0;
}
