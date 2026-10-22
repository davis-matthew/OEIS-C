/*
 * A007512  Primes of the form floor(e*10^k), i.e., formed by concatenation of
 *          an initial segment of the decimal expansion of e.
 * https://oeis.org/A007512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(e*10^(A064118(n)-1)), where A064118 = 1, 3, 7, 85,
 *            1781, 2780, 112280, 155025, ... floor(e*10^k) is computed
 *            exactly as floor(S*sum 1/i!) with S = 10^(k+G) guard digits,
 *            then divided by 10^G
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   e_floor_big, e_floor_small, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/e_floor_big.h"
#include "../utils/e_floor_small.h"

static const uint32_t A064118_tab[] = {1, 3, 7, 85, 1781, 2780, 112280, 155025};

seq_t a(idx_t n) {
    idx_t cnt = sizeof(A064118_tab) / sizeof(A064118_tab[0]);
    if (n < 1 || n > cnt) return 0;
    idx_t k = (idx_t)A064118_tab[n - 1] - 1;
    if (k <= 6) return u128_to_seq((unsigned __int128)e_floor_small(k));
    return e_floor_big(k);
}
