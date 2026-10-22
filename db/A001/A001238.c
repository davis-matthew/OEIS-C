/*
 * A001238  Differences of reciprocals of unity.
 * https://oeis.org/A001238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)!^5 * Sum_{k=1..n+1} (-1)^(k+1)*C(n+1,k)/k^5,
 *            computed with S_s(j) = Sum_{1<=i_1<=...<=i_s<=j} 1/(i_1*...*i_s)
 *            = Sum_{k=1..j} (-1)^(k+1)*C(j,k)/k^s, so a(n) = (n+1)!^5 *
 *            S_5(n+1). With V_s(j) = j!^5 * S_s(j): V_0(j) = j^5*V_0(j-1),
 *            V_s(j) = j^5*V_s(j-1) + V_{s-1}(j)/j (exact), V_0(0)...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t V[6];
static idx_t cur = 0;

seq_t a(idx_t n) {
    idx_t m = n + 1;
    int s;

    if (cur == 0 || m < cur) {
        V[0] = 1;
        for (s = 1; s <= 5; s++)
            V[s] = 0;
        cur = 0;
    }

    while (cur < m) {
        idx_t j = ++cur;
        seq_t sj = (seq_t)j;
        seq_t j5 = sj * sj * sj * sj * sj;
        V[0] = V[0] * j5;
        for (s = 1; s <= 5; s++)
            V[s] = V[s] * j5 + V[s - 1] / sj;
    }

    return V[5];
}
