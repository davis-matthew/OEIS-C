/*
 * A002476  Primes of the form 6m + 1.
 * https://oeis.org/A002476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   Primes of the form 6m + 1
 * Range:     u8 n <= 25, u16 n <= 3259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


static bool *sieve = NULL;
static idx_t sieve_size = 0;
static seq_t *primes_6m1 = NULL;
static idx_t primes_6m1_count = 0;
static idx_t primes_6m1_capacity = 0;

static void build_and_collect(idx_t limit)
{
	if (sieve) free(sieve);
	if (primes_6m1) free(primes_6m1);

	sieve = (bool *)malloc((size_t)limit + 1);
	if (!sieve) return;

	primes_6m1_capacity = (limit / 6) + 100;
	primes_6m1 = (seq_t *)malloc(sizeof(seq_t) * primes_6m1_capacity);
	if (!primes_6m1) {
		free(sieve);
		sieve = NULL;
		return;
	}

	for (idx_t i = 0; i <= limit; i++) {
		sieve[i] = true;
	}
	sieve[0] = sieve[1] = false;

	for (idx_t i = 2; i * i <= limit; i++) {
		if (sieve[i]) {
			for (idx_t j = i * i; j <= limit; j += i) {
				sieve[j] = false;
			}
		}
	}

	primes_6m1_count = 0;
	for (idx_t p = 7; p <= limit; p++) {
		if (sieve[p] && p % 6 == 1) {
			if (primes_6m1_count >= primes_6m1_capacity) {
				primes_6m1_capacity *= 2;
				primes_6m1 = (seq_t *)realloc(primes_6m1, sizeof(seq_t) * primes_6m1_capacity);
			}
			primes_6m1[primes_6m1_count++] = (seq_t)p;
		}
	}

	sieve_size = limit;
}

seq_t a(idx_t n)
{
	if (n == 0) return 0;

	if (n <= primes_6m1_count) {
		return primes_6m1[n - 1];
	}

	idx_t estimate = 30 * n + 100;
	if (estimate < 100) estimate = 100;

	build_and_collect(estimate);

	while (n > primes_6m1_count && sieve != NULL) {
		estimate = estimate * 2;
		build_and_collect(estimate);
	}

	if (n <= primes_6m1_count) {
		return primes_6m1[n - 1];
	}

	return 0;
}
