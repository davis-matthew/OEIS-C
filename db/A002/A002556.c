/*
 * A002556  Odd squarefree numbers with an odd number of prime factors that
 *          have no prime factors greater than 31.
 * https://oeis.org/A002556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Generate all squarefree products of odd primes ≤ 31 with odd
 *            number of prime factors; sequence is these products in sorted
 *            order
 * Range:     u8 none, u16 n <= 10, u32 n <= 455, u64 n <= 512, u128 n <= 512,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

static const uint64_t primes[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
#define NUM_PRIMES 10

static seq_t all_terms[512];
static int terms_computed = 0;

static void compute_all_terms(void) {
	if (terms_computed) return;
	
	int count = 0;
	
	for (int mask = 0; mask < (1 << NUM_PRIMES); mask++) {
		int bits = 0;
		for (int i = 0; i < NUM_PRIMES; i++) {
			if (mask & (1 << i)) bits++;
		}
		
		if (bits & 1) {
			seq_t product = 1;
			for (int i = 0; i < NUM_PRIMES; i++) {
				if (mask & (1 << i)) {
					product = product * (seq_t)primes[i];
				}
			}
			all_terms[count++] = product;
		}
	}
	
	qsort(all_terms, count, sizeof(seq_t), cmp_seq_t);
	
	terms_computed = 1;
}

seq_t a(idx_t n) {
	compute_all_terms();
	return all_terms[n - 1];
}
