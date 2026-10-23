/*
 * A107400  Numbers k such that 107*k + 109 is prime.
 * https://oeis.org/A107400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th nonnegative integer k such that 107·k + 109 is
 *            prime
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic Miller–Rabin test for 64‑bit integers. */
static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
	return (unsigned __int128)a * b % mod;
}

static uint64_t modpow64(uint64_t a, uint64_t d, uint64_t mod)
{
	uint64_t res = 1;
	while (d) {
		if (d & 1)
			res = modmul64(res, a, mod);
		a = modmul64(a, a, mod);
		d >>= 1;
	}
	return res;
}

static bool is_prime_uint64(uint64_t n)
{
	if (n < 2)
		return false;
	/* Small primes */
	static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
	                                 19ULL,23ULL,29ULL,31ULL,37ULL};
	for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
		if (n == small[i])
			return true;
		if (n % small[i] == 0)
			return false;
	}
	/* Write n‑1 as d·2^s with d odd */
	uint64_t d = n - 1, s = 0;
	while ((d & 1) == 0) {
		d >>= 1;
		++s;
	}
	/* Deterministic bases for 64‑bit */
	static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
	                                 450775ULL, 9780504ULL, 1795265022ULL};
	for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
		uint64_t a = bases[i] % n;
		if (a == 0)
			continue;
		uint64_t x = modpow64(a, d, n);
		if (x == 1 || x == n - 1)
			continue;
		bool cont = false;
		for (uint64_t r = 1; r < s; ++r) {
			x = modmul64(x, x, n);
			if (x == n - 1) {
				cont = true;
				break;
			}
		}
		if (!cont)
			return false;
	}
	return true;
}

seq_t a(idx_t n)
{
	/* Static cache of found k values */
	static seq_t *cache   = NULL;
	static idx_t cache_sz = 0;     /* number of stored terms */
	static idx_t cache_cap = 0;    /* allocation size of cache */
	static idx_t next_k    = 0;     /* next k to test */

	/* Ensure the cache array can hold at least one element now */
	if (cache_cap == 0) {
		cache_cap = 16;
		cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
	}

	/* Generate terms until we have at least n of them */
	while (cache_sz < n) {
		unsigned __int128 val128 = (unsigned __int128)107 * next_k + 109;
		uint64_t val = (uint64_t)val128; /* Safe for the expected range */

		if (is_prime_uint64(val)) {
			if (cache_sz == cache_cap) {
				cache_cap <<= 1;
				cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
			}
			cache[cache_sz++] = (seq_t)next_k;
		}
		++next_k;
	}

	return cache[n - 1];   /* a(1) is the first cached value */
}
