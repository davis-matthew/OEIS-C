/*
 * A072968  Least k>0 such that the last digit of (n+k)^(n+k) is the same as
 *          the last digit of n^n.
 * https://oeis.org/A072968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k>0 so that the last digit of (n+k)^(n+k) equals
 *            the last digit of n^n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t last_digit_from_remainders(uint8_t d, idx_t exp_mod4, idx_t exp_mod2)
{
    /* Return the last digit of d^e given d and e modulo its cycle length. */
    switch (d) {
    case 0: return 0;
    case 1: return 1;
    case 5: return 5;
    case 6: return 6;
    case 4: return (exp_mod2 == 0) ? 6 : 4;
    case 9: return (exp_mod2 == 0) ? 1 : 9;
    case 2:
        switch (exp_mod4) {
        case 0: return 6;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        }
    case 3:
        switch (exp_mod4) {
        case 0: return 1;
        case 1: return 3;
        case 2: return 9;
        case 3: return 7;
        }
    case 7:
        switch (exp_mod4) {
        case 0: return 1;
        case 1: return 7;
        case 2: return 9;
        case 3: return 3;
        }
    case 8:
        switch (exp_mod4) {
        case 0: return 6;
        case 1: return 8;
        case 2: return 4;
        case 3: return 2;
        }
    }
    return 0; /* unreachable */
}

seq_t a(idx_t n)
{
    /* Pre‑compute last digit of n^n using remainders. */
    uint8_t d_n = (uint8_t)(n % 10);
    idx_t  n_mod4 = n % 4;
    idx_t  n_mod2 = n % 2;
    seq_t target = last_digit_from_remainders(d_n, n_mod4, n_mod2);

    /* Explore k=1…20 – the sequence is periodic with period 20. */
    for (idx_t k = 1; k <= 20; ++k) {
        uint8_t d_m = (uint8_t)((d_n + (k % 10)) % 10);
        idx_t  m_mod4 = (n_mod4 + (k % 4)) % 4;
        idx_t  m_mod2 = (n_mod2 + (k % 2)) % 2;
        seq_t cur = last_digit_from_remainders(d_m, m_mod4, m_mod2);
        if (cur == target)
            return (seq_t)k;
    }

    /* For safety – this should never happen. */
    return (seq_t)0;
}
