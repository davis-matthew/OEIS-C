/*
 * A003809  Order of simple Chevalley group A_4(q), q = prime power.
 * https://oeis.org/A003809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^10*(q^2-1)*(q^3-1)*(q^4-1)*(q^5-1)/gcd(5,q-1), where q
 *            = A000961(n+1)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *pp_list = NULL;
static idx_t pp_count = 0;
static uint64_t pp_limit = 0;

/* list prime powers > 1 up to limit, in increasing order */
static void build_prime_powers(uint64_t limit) {
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    uint64_t i, j;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    free(pp_list);
    pp_list = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    pp_count = 0;
    for (i = 2; i <= limit; i++) {
        uint64_t p = spf[i], m = i;
        while (m % p == 0) m /= p;
        if (m == 1) pp_list[pp_count++] = i;
    }
    pp_limit = limit;
    free(spf);
}

seq_t a(idx_t n) {
    uint64_t q, d;
    seq_t Q, q2, q3, q4, q5, q10, result;

    if (pp_limit == 0) build_prime_powers(200000);
    while (n > pp_count) build_prime_powers(pp_limit * 2);

    q = pp_list[n - 1];
    d = ((q - 1) % 5 == 0) ? 5 : 1;

    Q = (seq_t)q;
    q2 = Q * Q;
    q3 = q2 * Q;
    q4 = q2 * q2;
    q5 = q4 * Q;
    q10 = q5 * q5;

    if (d == 5) {
        /* q ≡ 1 mod 5, so 5 divides q-1 and hence q^5-1 */
        result = q10 * (q2 - 1) * (q3 - 1) * (q4 - 1) * ((q5 - 1) / 5);
    } else {
        result = q10 * (q2 - 1) * (q3 - 1) * (q4 - 1) * (q5 - 1);
    }
    return result;
}
