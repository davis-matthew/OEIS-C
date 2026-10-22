/*
 * A003849  The infinite Fibonacci word (start with 0, apply 0->01, 1->0, take
 *          limit).
 * https://oeis.org/A003849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Fibonacci word S(n)=S(n-1)S(n-2), S(0)="0", S(1)="01"; a(n) is
 *            n-th char of S(∞)
 * Range:     u8 n <= 9465, u16 n <= 9465, u32 n <= 9465, u64 n <= 9465,
 *            u128 n <= 9465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t fib[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073, 4807526976, 7778742049, 12586269025, 20365011074, 32951280099, 53316291173, 86267571272, 139583862445, 225851433717, 365435296162, 591286729879, 956722026041, 1548008755920, 2504730781961, 4052739537881, 6557470319842, 10610209857723, 17167680177565, 27777890035288, 44945570212853, 72723460248141, 117669030460994, 190392490709135, 308061521170129, 498454011879264, 806515533049393};

static int fib_count = sizeof(fib) / sizeof(fib[0]);

static seq_t a_recursive(int k, uint64_t pos) {
	if (k == 0) {
		return 0;
	} else if (k == 1) {
		return (pos == 0) ? 0 : 1;
	} else {
		uint64_t len_k_1 = fib[k];
		if (pos < len_k_1) {
			return a_recursive(k - 1, pos);
		} else {
			return a_recursive(k - 2, pos - len_k_1);
		}
	}
}

seq_t a(idx_t n) {
	int k = 0;
	while (k + 1 < fib_count && fib[k + 1] <= n) {
		k++;
	}
	return a_recursive(k, n);
}
