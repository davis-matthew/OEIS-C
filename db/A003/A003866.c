/*
 * A003866  Degrees of irreducible representations of alternating group A_11.
 * https://oeis.org/A003866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest degree of an irreducible character of A_11,
 *            sorted ascending (31 terms). For each partition L of 11, f(L) =
 *            11!/prod(hook lengths). Each conjugate pair L != L' gives one
 *            degree f(L); each self-conjugate L gives two degrees f(L)/2
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   cmp_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_ll.h"

static long long degs[64];
static int ndegs = 0;
static int built = 0;
static int parts[12];

static void process(int k)
{
	int conj[12];
	int i, j, cmp;
	long long prod = 1, fact = 1, f;

	for (j = 0; j < parts[0]; j++) {
		int c = 0;
		for (i = 0; i < k; i++)
			if (parts[i] > j)
				c++;
		conj[j] = c;
	}
	/* compare partition with its conjugate */
	cmp = 0;
	for (i = 0; i < 12 && cmp == 0; i++) {
		int p = (i < k) ? parts[i] : 0;
		int q = (i < parts[0]) ? conj[i] : 0;
		if (p > q)
			cmp = 1;
		else if (p < q)
			cmp = -1;
	}
	if (cmp < 0)
		return;
	for (i = 0; i < k; i++)
		for (j = 0; j < parts[i]; j++)
			prod *= (long long)(parts[i] - j - 1 + conj[j] - i - 1 + 1);
	for (i = 2; i <= 11; i++)
		fact *= i;
	f = fact / prod;
	if (cmp == 0) {
		degs[ndegs++] = f / 2;
		degs[ndegs++] = f / 2;
	} else {
		degs[ndegs++] = f;
	}
}

static void gen(int remaining, int maxpart, int k)
{
	int p;
	if (remaining == 0) {
		process(k);
		return;
	}
	for (p = (remaining < maxpart ? remaining : maxpart); p >= 1; p--) {
		parts[k] = p;
		gen(remaining - p, p, k + 1);
	}
}

seq_t a(idx_t n)
{
	seq_t r = 0;
	if (!built) {
		ndegs = 0;
		gen(11, 11, 0);
		qsort(degs, (size_t)ndegs, sizeof(long long), cmp_ll);
		built = 1;
	}
	if (n < 1 || n > (idx_t)ndegs)
		return r;
	r += (idx_t)degs[n - 1];
	return r;
}
