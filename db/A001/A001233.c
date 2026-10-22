/*
 * A001233  Unsigned Stirling numbers of first kind s(n,6).
 * https://oeis.org/A001233
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2*n - 7)*a(n-1) - 5*(3*n^2 - 24*n + 49)*a(n-2) +
 *            10*(2*n - 9)*(n^2 - 9*n + 21)*a(n-3) - (15*n^4 - 300*n^3 +
 *            2265*n^2 - 7650*n + 9751)*a(n-4) + (2*n - 11)*(n^2 - 11*n +
 *            31)*(3*n^2 - 33*n + 91)*a(n-5) - (n-6)^6*a(n-6), with a(6) = 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_SIZE 10001

static seq_t cache_vals[CACHE_SIZE];
static int cache_computed = 0;

static void build_cache(void) {
	cache_vals[0] = 1;
	
	for (idx_t i = 1; i < CACHE_SIZE; i++) {
		idx_t n = i + 6;
		seq_t m = (seq_t)n;
		
		seq_t a_i_1 = cache_vals[i - 1];
		seq_t a_i_2 = (i >= 2) ? cache_vals[i - 2] : 0;
		seq_t a_i_3 = (i >= 3) ? cache_vals[i - 3] : 0;
		seq_t a_i_4 = (i >= 4) ? cache_vals[i - 4] : 0;
		seq_t a_i_5 = (i >= 5) ? cache_vals[i - 5] : 0;
		seq_t a_i_6 = (i >= 6) ? cache_vals[i - 6] : 0;
		
		seq_t term1 = 3 * (2*m - 7) * a_i_1;
		seq_t term2 = 5 * (3*m*m - 24*m + 49) * a_i_2;
		seq_t term3 = 10 * (2*m - 9) * (m*m - 9*m + 21) * a_i_3;
		seq_t c4 = 15*m*m*m*m - 300*m*m*m + 2265*m*m - 7650*m + 9751;
		seq_t term4 = c4 * a_i_4;
		seq_t c5 = (2*m - 11) * (m*m - 11*m + 31) * (3*m*m - 33*m + 91);
		seq_t term5 = c5 * a_i_5;
		seq_t nm6 = m - 6;
		seq_t c6 = nm6 * nm6 * nm6 * nm6 * nm6 * nm6;
		seq_t term6 = c6 * a_i_6;
		
		cache_vals[i] = term1 - term2 + term3 - term4 + term5 - term6;
	}
	
	cache_computed = 1;
}

seq_t a(idx_t n) {
	if (n < 6) return 0;
	if (n == 6) return 1;
	
	if (n - 6 < CACHE_SIZE) {
		if (!cache_computed) build_cache();
		return cache_vals[n - 6];
	}
	
	if (!cache_computed) build_cache();
	
	seq_t v[6];
	for (int j = 0; j < 6; j++) {
		v[j] = cache_vals[CACHE_SIZE - 6 + j];
	}
	
	for (idx_t i = CACHE_SIZE + 6; i <= n; i++) {
		seq_t m = (seq_t)i;
		
		seq_t term1 = 3 * (2*m - 7) * v[5];
		seq_t term2 = 5 * (3*m*m - 24*m + 49) * v[4];
		seq_t term3 = 10 * (2*m - 9) * (m*m - 9*m + 21) * v[3];
		seq_t c4 = 15*m*m*m*m - 300*m*m*m + 2265*m*m - 7650*m + 9751;
		seq_t term4 = c4 * v[2];
		seq_t c5 = (2*m - 11) * (m*m - 11*m + 31) * (3*m*m - 33*m + 91);
		seq_t term5 = c5 * v[1];
		seq_t nm6 = m - 6;
		seq_t c6 = nm6 * nm6 * nm6 * nm6 * nm6 * nm6;
		seq_t term6 = c6 * v[0];
		
		seq_t result = term1 - term2 + term3 - term4 + term5 - term6;
		
		for (int j = 0; j < 5; j++) {
			v[j] = v[j+1];
		}
		v[5] = result;
	}
	
	return v[5];
}
