/*
 * A085779  Smallest m such that the triangular number A000217(n) divides m!.
 * https://oeis.org/A085779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer m such that (n*(n+1))/2 divides m!
 * Range:     u8 n <= 22, u16 n <= 1205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t min_m_for_prime_power(seq_t p, seq_t e)
{
    /* Find minimal m with v_p(m!) >= e  (Legendre's formula) */
    seq_t low = 1;
    seq_t high = e;   /* start with at least e */
    /* Grow high until it satisfies the condition */
    while (1) {
        seq_t sum = 0;
        seq_t div = high;
        while (div) {
            div /= p;
            sum += div;
        }
        if (sum >= e)
            break;
        high *= 2;   /* double until enough */
    }
    /* Binary search between low and high */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t sum = 0;
        seq_t div = mid;
        while (div) {
            div /= p;
            sum += div;
        }
        if (sum >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n >= 1 according to the problem statement */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number */
    seq_t tmp = t;
    seq_t answer = 1;

    /* Factorise t by trial division */
    for (idx_t i = 2; (seq_t)i * (seq_t)i <= tmp; ++i) {
        if (tmp % (seq_t)i == 0) {
            seq_t exp = 0;
            while (tmp % (seq_t)i == 0) {
                tmp /= (seq_t)i;
                ++exp;
            }
            seq_t m_for = min_m_for_prime_power((seq_t)i, exp);
            if (m_for > answer)
                answer = m_for;
        }
    }

    /* Remaining prime factor, if any */
    if (tmp > 1) {
        seq_t m_for = min_m_for_prime_power(tmp, 1);
        if (m_for > answer)
            answer = m_for;
    }

    return answer;
}
