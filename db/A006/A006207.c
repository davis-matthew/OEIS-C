/*
 * A006207  Generalized Fibonacci numbers A_{n,2}.
 * https://oeis.org/A006207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} mu(n/d) * s(d), where s(n) = s(n-1) +
 *            s(n-2) - s(n-3) + s(n-4), s(0) = 4, s(1) = 1, s(2) = 3, s(3) = 1
 *            (s(n) is the power sum of the roots of x^4 = x^3 + x^2 - x + 1)
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

static int mobius_small(idx_t m) {
	int r = 1;
	for (idx_t p = 2; p * p <= m; p++) {
		if (m % p == 0) {
			m /= p;
			if (m % p == 0) return 0;
			r = -r;
		}
	}
	if (m > 1) r = -r;
	return r;
}

static unsigned __int128 *sw_cache = NULL;
static idx_t sw_len = 0;

static void ensure_w(idx_t n) {
	if (n < sw_len) return;
	idx_t nl = (n + 1 > 2 * sw_len) ? n + 1 : 2 * sw_len;
	if (nl < 8) nl = 8;
	sw_cache = (unsigned __int128 *)realloc(sw_cache, (size_t)nl * sizeof(unsigned __int128));
	for (idx_t i = sw_len; i < nl; i++) {
		if (i == 0) sw_cache[i] = 4;
		else if (i == 1) sw_cache[i] = 1;
		else if (i == 2) sw_cache[i] = 3;
		else if (i == 3) sw_cache[i] = 1;
		else sw_cache[i] = sw_cache[i - 1] + sw_cache[i - 2] + sw_cache[i - 4] - sw_cache[i - 3];
	}
	sw_len = nl;
}

static seq_t *sb_cache = NULL;
static idx_t sb_len = 0;

static void ensure_b(idx_t n) {
	if (n < sb_len) return;
	idx_t nl = (n + 1 > 2 * sb_len) ? n + 1 : 2 * sb_len;
	if (nl < 8) nl = 8;
	sb_cache = grow_zero(sb_cache, sb_len, nl);
	for (idx_t i = sb_len; i < nl; i++) {
		if (i == 0) sb_cache[i] = 4;
		else if (i == 1) sb_cache[i] = 1;
		else if (i == 2) sb_cache[i] = 3;
		else if (i == 3) sb_cache[i] = 1;
		else {
			seq_t t = sb_cache[i - 1] + sb_cache[i - 2];
			t += sb_cache[i - 4];
			t -= sb_cache[i - 3];
			sb_cache[i] = t;
		}
	}
	sb_len = nl;
}

seq_t a(idx_t n) {
	if (n < 1) return 0;
	if (seq_is_fixed_width()) {
		ensure_w(n);
		unsigned __int128 pos = 0, neg = 0;
		for (idx_t d = 1; d <= n; d++) {
			if (n % d != 0) continue;
			int mu = mobius_small(n / d);
			if (mu > 0) pos += sw_cache[d];
			else if (mu < 0) neg += sw_cache[d];
		}
		return u128_to_seq((pos - neg) / (unsigned __int128)n);
	} else {
		ensure_b(n);
		seq_t pos = 0, neg = 0;
		for (idx_t d = 1; d <= n; d++) {
			if (n % d != 0) continue;
			int mu = mobius_small(n / d);
			if (mu > 0) pos += sb_cache[d];
			else if (mu < 0) neg += sb_cache[d];
		}
		seq_t r = pos - neg;
		r /= n;
		return r;
	}
}
