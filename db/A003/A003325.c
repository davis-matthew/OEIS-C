/*
 * A003325  Numbers that are the sum of 2 positive cubes.
 * https://oeis.org/A003325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search -- list all a^3 + b^3 <= X with 1 <= a <= b, sort and
 *            dedupe, enlarging X until at least n distinct sums exist
 * Range:     u8 n <= 18, u16 n <= 706, u32 n <= 9957, u64 n <= 9957,
 *            u128 n <= 9957, big any n
 * Helpers:   a3325_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3325_cmp.h"

static uint64_t *a3325_tab = NULL;
static idx_t a3325_cnt = 0;

static void a3325_build(uint64_t X) {
	uint64_t B = 1, i, j, cap = 0, cnt = 0;
	uint64_t *v;
	while ((B + 1) * (B + 1) * (B + 1) < X) B++;
	for (i = 1; i <= B; i++) {
		uint64_t i3 = i * i * i;
		if (2 * i3 > X) break;
		for (j = i; j <= B; j++) {
			if (i3 + j * j * j > X) break;
			cap++;
		}
	}
	v = (uint64_t *)malloc((cap + 1) * sizeof(uint64_t));
	for (i = 1; i <= B; i++) {
		uint64_t i3 = i * i * i;
		if (2 * i3 > X) break;
		for (j = i; j <= B; j++) {
			uint64_t s = i3 + j * j * j;
			if (s > X) break;
			v[cnt++] = s;
		}
	}
	qsort(v, (size_t)cnt, sizeof(uint64_t), a3325_cmp);
	{
		uint64_t u = 0;
		for (i = 0; i < cnt; i++)
			if (u == 0 || v[i] != v[u - 1]) v[u++] = v[i];
		cnt = u;
	}
	free(a3325_tab);
	a3325_tab = v;
	a3325_cnt = (idx_t)cnt;
}

seq_t a(idx_t n) {
	if (n == 0) return 0;
	if (n > a3325_cnt) {
		uint64_t X = 1024;
		while (X < (uint64_t)n * 16) X *= 2;
		for (;;) {
			a3325_build(X);
			if (a3325_cnt >= n) break;
			X *= 4;
		}
	}
	return (seq_t)a3325_tab[n - 1];
}
