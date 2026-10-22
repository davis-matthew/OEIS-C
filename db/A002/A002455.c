/*
 * A002455  Central factorial numbers: unsigned 1st subdiagonal of A182867.
 * https://oeis.org/A002455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*(2*n^2 - 2*n + 1)*a(n-1) - 16*(n-1)^4*a(n-2), a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
	if (n == 0) return 0;
	if (n == 1) return 1;
	
	if (memo == NULL || memo_len <= n) {
		idx_t new_len = (memo_len == 0) ? 
			((n < 100) ? 100 : n + 1) : 
			(memo_len * 2 < n + 1 ? n + 1 : memo_len * 2);
		
		seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
		bool *new_computed = (bool *)realloc(computed, new_len * sizeof(bool));
		
		if (new_memo == NULL || new_computed == NULL) return 0;
		
		if (memo == NULL) {
			new_memo[0] = 0;
			new_memo[1] = 1;
			new_computed[0] = true;
			new_computed[1] = true;
			for (idx_t i = 2; i < new_len; i++) {
				new_computed[i] = false;
			}
		} else {
			for (idx_t i = memo_len; i < new_len; i++) {
				new_computed[i] = false;
			}
		}
		
		memo = new_memo;
		computed = new_computed;
		memo_len = new_len;
	}
	
	for (idx_t i = 2; i <= n; i++) {
		if (!computed[i]) {
			seq_t i_seq = (seq_t)i;
			seq_t coeff1 = 4 * (2 * i_seq * i_seq - 2 * i_seq + 1);
			seq_t term1 = coeff1 * memo[i - 1];
			
			seq_t nm1_seq = (seq_t)(i - 1);
			seq_t coeff2 = 16 * nm1_seq * nm1_seq * nm1_seq * nm1_seq;
			seq_t term2 = coeff2 * memo[i - 2];
			
			memo[i] = term1 - term2;
			computed[i] = true;
		}
	}
	
	return memo[n];
}
