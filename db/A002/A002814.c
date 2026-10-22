/*
 * A002814  For n > 1: a(n) = a(n-1)^3 + 3*a(n-1)^2 - 3; a(0) = 1, a(1) = 2.
 * https://oeis.org/A002814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1)^3 + 3*a(n-1)^2 - 3, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *memo = NULL;
static unsigned char *computed = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
	if (memo == NULL) {
		memo_len = (n < 64) ? 128 : ((n + 1) * 2);
		memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
		computed = (unsigned char *)malloc(memo_len);
		memset(computed, 0, memo_len);
	} else if (n >= memo_len) {
		idx_t new_len = memo_len * 2;
		while (new_len <= n) new_len *= 2;
		seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
		unsigned char *new_computed = (unsigned char *)malloc(new_len);
		memcpy(new_memo, memo, memo_len * sizeof(seq_t));
		memcpy(new_computed, computed, memo_len);
		memset(new_computed + memo_len, 0, new_len - memo_len);
		free(memo);
		free(computed);
		memo = new_memo;
		computed = new_computed;
		memo_len = new_len;
	}
	
	for (idx_t i = 0; i <= n; i++) {
		if (!computed[i]) {
			if (i == 0) {
				memo[i] = (seq_t)1;
			} else if (i == 1) {
				memo[i] = (seq_t)2;
			} else {
				seq_t prev = memo[i - 1];
				seq_t sq = prev * prev;
				memo[i] = sq * prev + 3 * sq - 3;
			}
			computed[i] = 1;
		}
	}
	
	return memo[n];
}
